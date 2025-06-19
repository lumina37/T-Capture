#pragma once

#include <mutex>

#include <mfidl.h>
#include <mfreadwrite.h>
#include <shlwapi.h>

#include "tcap/camera/mf/reader/async/callback.hpp"
#include "tcap/camera/mf/reader/async/concepts.hpp"
#include "tcap/camera/mf/reader/core.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::mf {

class SampleCallbackBase : public IMFSourceReaderCallback {
public:
    virtual ~SampleCallbackBase() noexcept = default;

    /* IUnknown impl (Don't need to care) */
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv) override {
        static const QITAB qit[] = {
            QITABENT(SampleCallbackBase, IMFSourceReaderCallback),
            {nullptr},
        };
        return QISearch(this, qit, iid, ppv);
    }
    STDMETHODIMP_(ULONG) AddRef() override { return 1; }
    STDMETHODIMP_(ULONG) Release() override { return 0; }
    /* IUnknown impl */
};

template <typename TAwaitable_>
class SampleCallback_ : public SampleCallbackBase {
public:
    using TAwaitable = TAwaitable_;

    SampleCallback_() noexcept = default;
    SampleCallback_(const SampleCallback_&) = delete;
    SampleCallback_& operator=(const SampleCallback_&) = delete;
    SampleCallback_(SampleCallback_&& rhs) noexcept = delete;
    SampleCallback_& operator=(SampleCallback_&& rhs) noexcept = delete;
    ~SampleCallback_() noexcept override = default;

    void adoptReaderBox(ReaderBox&& readerBox) noexcept { readerBox_ = std::move(readerBox); }
    void setCurrentAwaitable(TAwaitable* awaitable) noexcept { currentAwaitable_ = awaitable; }
    std::mutex& getMutex() noexcept { return mutex_; }

    [[nodiscard]] const ReaderBox& getReaderBox() const noexcept { return readerBox_; }
    [[nodiscard]] ReaderBox& getReaderBox() noexcept { return readerBox_; }

    /* IMFSourceReaderCallback impl */
    // !!! Main logic of the reader callback !!!
    // In `OnReadSample`, if `pSample` is available,
    // then we will call `SampleAwaitable::setSampleBoxRes` to set the coroutine result
    // and finally call `SampleAwaitable::resume` to resume the coroutine
    STDMETHODIMP OnReadSample(HRESULT hr, DWORD streamIndex, DWORD streamFlags, LONGLONG timestamp,
                              IMFSample* pSample) noexcept override;
    STDMETHODIMP OnEvent(DWORD, IMFMediaEvent*) noexcept override { return S_OK; }
    STDMETHODIMP OnFlush(DWORD) noexcept override { return S_OK; }
    /* IMFSourceReaderCallback impl */

private:
    ReaderBox readerBox_;
    TAwaitable* currentAwaitable_;
    std::mutex mutex_;
};

template <typename TAwaitable>
STDMETHODIMP SampleCallback_<TAwaitable>::OnReadSample(HRESULT hr, [[maybe_unused]] DWORD streamIndex,
                                                       DWORD streamFlags, LONGLONG timestamp,
                                                       IMFSample* pSample) noexcept {
    if (FAILED(hr)) {
        {
            auto lock = std::lock_guard{currentAwaitable_->getMutex()};
            currentAwaitable_->setSampleBoxRes(std::unexpected{Error{ECate::eMF, hr}});
        }
        currentAwaitable_->resume();
        return S_OK;
    }

    {
        auto lock = std::lock_guard{currentAwaitable_->getMutex()};
        if (pSample == nullptr) {
            currentAwaitable_->setSampleBoxRes(
                std::unexpected{Error{ECate::eTCap, ECode::eUnexValue, "pSample is nullptr"}});
        } else {
            SampleBox sampleBox = SampleBox::create(pSample, streamFlags, timestamp).value();
            currentAwaitable_->setSampleBoxRes(std::move(sampleBox));
        }
    }

    currentAwaitable_->resume();  // goto `SampleAwaitable::await_resume`
    return S_OK;
}

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader/async/callback.cpp"
#endif
