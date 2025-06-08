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

    // Helper functions
    void setPReader(IMFSourceReader* pReader) noexcept { pReader_ = pReader; }
    void setCurrentAwaitable(SampleAwaitable* awaitable) noexcept { currentAwaitable_ = awaitable; }
    void _sample() {
        std::unique_lock lock(mutex_);
        HRESULT hr = pReader_->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, nullptr, nullptr, nullptr, nullptr);
        if (FAILED(hr)) {
            err_ = {hr, "pReader_->ReadSample failed"};
        }
    }
    /* Awaitable impl */

    /* IUnknown impl */
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv) override {
        static const QITAB qit[] = {
            QITABENT(SampleCallback, IMFSourceReaderCallback),
            {nullptr},
        };
        return QISearch(this, qit, iid, ppv);
    }
    STDMETHODIMP_(ULONG) AddRef() override { return InterlockedIncrement(&refCount_); }
    STDMETHODIMP_(ULONG) Release() override {
        ULONG uCount = InterlockedDecrement(&refCount_);
        if (uCount == 0) {
            delete this;
        }
        return uCount;
    }
    /* IUnknown impl */

    /* IMFSourceReaderCallback impl */
    STDMETHODIMP OnReadSample(HRESULT hr, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp,
                              IMFSample* pSample) override;
    STDMETHODIMP OnEvent(DWORD, IMFMediaEvent*) override { return S_OK; }
    STDMETHODIMP OnFlush(DWORD) override { return S_OK; }
    /* IMFSourceReaderCallback impl */

    friend class SampleAwaitable;

private:
    IMFSourceReader* pReader_;
    std::mutex mutex_;
    SampleAwaitable* currentAwaitable_;

    LONG refCount_;
    Error err_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/callback.cpp"
#endif
