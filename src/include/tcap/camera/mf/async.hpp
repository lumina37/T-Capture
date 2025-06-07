#pragma once

#include <coroutine>
#include <mutex>

#include <mfidl.h>
#include <mfreadwrite.h>
#include <shlwapi.h>

#include "tcap/camera/mf/sample.hpp"

namespace tcap::mf {
class SampleCallback : public IMFSourceReaderCallback {
public:
    SampleCallback() noexcept : pReader_(nullptr) {}
    SampleCallback(IMFSourceReader* pReader) noexcept : pReader_(pReader) {}
    SampleCallback(SampleCallback&& rhs) noexcept : pReader_(rhs.pReader_) {}
    virtual ~SampleCallback() noexcept = default;

    /* Awaitable impl */
    struct SampleAwaitable {
        SampleAwaitable(SampleCallback* pCallback) noexcept : pCallback_(pCallback) {}

        SampleCallback* pCallback_;

        // Results
        SampleBox sampleBox_;
        DWORD actualStreamIndex_;
        DWORD streamFlags_;
        LONGLONG timestamp_;

        std::coroutine_handle<> handle_;

        bool await_ready() { return false; }
        void await_suspend(std::coroutine_handle<> handle) {
            handle_ = handle;
            pCallback_->_sample(this);
        }

        std::expected<SampleBox, Error> await_resume() {
            if (FAILED(pCallback_->hr_)) {
                return std::unexpected{Error{pCallback_->hr_, std::move(pCallback_->errMsg_)}};
            }
            return std::move(sampleBox_);
        }
    };

    // !!! Users should only call this
    SampleAwaitable sample() { return {this}; }

    // Helper function to actually call `ReadSample` and remember the current awaitable
    void _sample(SampleAwaitable* awaitable) {
        std::unique_lock lock(mutex_);
        currentAwaitable_ = awaitable;
        hr_ = pReader_->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, nullptr, nullptr, nullptr, nullptr);
        if (FAILED(hr_)) {
            errMsg_ = "pReader_->ReadSample failed";
        }
    }
    /* Awaitable impl */

    // IUnknown methods
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv) {
        static const QITAB qit[] = {
            QITABENT(SampleCallback, IMFSourceReaderCallback),
            {0},
        };
        return QISearch(this, qit, iid, ppv);
    }
    STDMETHODIMP_(ULONG) AddRef() { return 1; }
    STDMETHODIMP_(ULONG) Release() { return 0; }

    /* IMFSourceReaderCallback impl */
    STDMETHODIMP OnReadSample(HRESULT hr, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp,
                              IMFSample* pSample) override {
        if (FAILED(hr_)) return S_OK;
        if (currentAwaitable_ == nullptr) return S_OK;
        if (pSample == nullptr) return S_OK;

        std::unique_lock lock(mutex_);
        SampleAwaitable* awaitable = std::exchange(currentAwaitable_, nullptr);

        auto newSampleBoxRes = SampleBox::create(pSample);
        std::exchange(awaitable->sampleBox_, std::move(newSampleBoxRes.value()));
        awaitable->actualStreamIndex_ = dwStreamIndex;
        awaitable->streamFlags_ = dwStreamFlags;
        awaitable->timestamp_ = llTimestamp;

        // Resume coroutine
        awaitable->handle_.resume();

        return S_OK;
    }
    STDMETHODIMP OnEvent(DWORD, IMFMediaEvent*) override { return S_OK; }
    STDMETHODIMP OnFlush(DWORD) override { return S_OK; }
    /* IMFSourceReaderCallback impl */

    IMFSourceReader* pReader_;
    std::mutex mutex_;
    SampleAwaitable* currentAwaitable_ = nullptr;

    HRESULT hr_ = S_OK;
    std::string errMsg_{};
};

}  // namespace tcap::mf
