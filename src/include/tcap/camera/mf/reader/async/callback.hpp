#pragma once

#include <mutex>

#include <mfidl.h>
#include <mfreadwrite.h>
#include <shlwapi.h>

#include "tcap/camera/mf/reader/async/awaitable.hpp"

namespace tcap::mf {

class SampleCallbackBase : public IMFSourceReaderCallback {
public:
    virtual ~SampleCallbackBase() noexcept = default;

    /* IUnknown impl (Don't need to care) */
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv) override {
        static const QITAB qit[] = {
            QITABENT(SampleCallback, IMFSourceReaderCallback),
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
    SampleCallback() noexcept;
    SampleCallback(const SampleCallback&) = delete;
    SampleCallback& operator=(const SampleCallback&) = delete;
    SampleCallback(SampleCallback&& rhs) noexcept;
    SampleCallback& operator=(SampleCallback&& rhs) noexcept;
    virtual ~SampleCallback() noexcept = default;

    void setPReader(IMFSourceReader* pReader) noexcept { pReader_ = pReader; }
    void setCurrentAwaitable(SampleAwaitable* awaitable) noexcept { currentAwaitable_ = awaitable; }

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
    void sampleNonBlock() noexcept;

    IMFSourceReader* pReader_;
    SampleAwaitable* currentAwaitable_;
    std::mutex mutex_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader/async/callback.cpp"
#endif
