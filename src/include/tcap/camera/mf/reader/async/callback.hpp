#pragma once

#include <mutex>

#include <mfidl.h>
#include <mfreadwrite.h>
#include <shlwapi.h>

#include "tcap/camera/mf/reader/async/awaitable.hpp"
#include "tcap/camera/mf/reader/async/callback.hpp"
#include "tcap/camera/mf/reader/core.hpp"

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

class SampleCallback : public SampleCallbackBase {
public:
    SampleCallback() noexcept = default;
    SampleCallback(const SampleCallback&) = delete;
    SampleCallback& operator=(const SampleCallback&) = delete;
    SampleCallback(SampleCallback&& rhs) noexcept = delete;
    SampleCallback& operator=(SampleCallback&& rhs) noexcept = delete;
    ~SampleCallback() noexcept override = default;

    void adoptReaderBox(ReaderBox&& readerBox) noexcept { readerBox_ = std::move(readerBox); }
    void setCurrentAwaitable(SampleAwaitable_<SampleCallback>* awaitable) noexcept { currentAwaitable_ = awaitable; }
    void lock() noexcept { mutex_.lock(); }
    void unlock() noexcept { mutex_.unlock(); }
    auto autoLock() noexcept { return std::lock_guard{mutex_}; }

    [[nodiscard]] const ReaderBox& getReaderBox() const noexcept { return readerBox_; }
    [[nodiscard]] ReaderBox& getReaderBox() noexcept { return readerBox_; }

    /* IMFSourceReaderCallback impl */
    // !!! Main logic of the reader callback !!!
    // In `OnReadSample`, if `pSample` is available,
    // then we will call `SampleAwaitable::setPSample` to set the coroutine result
    // and finally call `SampleAwaitable::resume` to resume the coroutine
    STDMETHODIMP OnReadSample(HRESULT hr, DWORD streamIndex, DWORD streamFlags, LONGLONG timestamp,
                              IMFSample* pSample) noexcept override;
    STDMETHODIMP OnEvent(DWORD, IMFMediaEvent*) noexcept override { return S_OK; }
    STDMETHODIMP OnFlush(DWORD) noexcept override { return S_OK; }
    /* IMFSourceReaderCallback impl */

    friend class SampleAwaitable;

private:
    ReaderBox readerBox_;
    SampleAwaitable_<SampleCallback>* currentAwaitable_;
    std::mutex mutex_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader/async/callback.cpp"
#endif
