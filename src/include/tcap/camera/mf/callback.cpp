#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/callback.hpp"
#endif

namespace tcap::mf {

SampleCallback::SampleCallback(SampleCallback&& rhs) noexcept
    : pReader_(std::exchange(rhs.pReader_, nullptr)),
      currentAwaitable_(std::exchange(rhs.currentAwaitable_, nullptr)),
      err_(std::move(rhs.err_)) {}

STDMETHODIMP SampleCallback::OnReadSample(HRESULT hr, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp,
                                          IMFSample* pSample) noexcept {
    if (FAILED(hr)) {
        err_ = {hr, "OnReadSample failed"};
        return S_OK;
    }
    if (pSample == nullptr) {
        sampleNonBlock();
        return S_OK;
    }

    pSample->AddRef();
    currentAwaitable_->setPSample(pSample);
    currentAwaitable_->resume();  // goto `SampleAwaitable::await_resume`

    return S_OK;
}

void SampleCallback::sampleNonBlock() noexcept {
    std::unique_lock lock(mutex_);
    HRESULT hr = pReader_->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, nullptr, nullptr, nullptr, nullptr);
    if (FAILED(hr)) {
        err_ = {hr, "pReader_->ReadSample failed"};
    }
}

}  // namespace tcap::mf
