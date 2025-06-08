#pragma once

#include <mutex>

#include <mfidl.h>
#include <mfreadwrite.h>
#include <shlwapi.h>

#include "tcap/camera/mf/coroutine.hpp"
#include "tcap/camera/mf/sample.hpp"

namespace tcap::mf {

class SampleCallback : public IMFSourceReaderCallback {
public:
    SampleCallback() noexcept = default;
    SampleCallback(SampleCallback&& rhs) noexcept;
    virtual ~SampleCallback() = default;

    void setPReader(IMFSourceReader* pReader) noexcept { pReader_ = pReader; }
    void setCurrentAwaitable(SampleAwaitable* awaitable) noexcept { currentAwaitable_ = awaitable; }
    void _sampleNonBlock() {
        std::unique_lock lock(mutex_);
        HRESULT hr = pReader_->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, nullptr, nullptr, nullptr, nullptr);
        if (FAILED(hr)) {
            err_ = {hr, "pReader_->ReadSample failed"};
        }
    }

    /* IMFSourceReaderCallback impl */
    // !!! Main logic of the reader callback !!!
    // In `OnReadSample`, if `pSample` is available,
    // then we will call `SampleAwaitable::setPSample` to set the coroutine result
    // and finally call `SampleAwaitable::resume` to resume the coroutine
    STDMETHODIMP OnReadSample(HRESULT hr, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp,
                              IMFSample* pSample) override;
    STDMETHODIMP OnEvent(DWORD, IMFMediaEvent*) override { return S_OK; }
    STDMETHODIMP OnFlush(DWORD) override { return S_OK; }
    /* IMFSourceReaderCallback impl */

    friend class SampleAwaitable;

private:
    IMFSourceReader* pReader_;
    SampleAwaitable* currentAwaitable_;
    Error err_;
    std::mutex mutex_;

public:
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

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/callback.cpp"
#endif
