#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/callback.hpp"
#endif

namespace tcap::mf {

SampleCallback::SampleCallback(SampleCallback&& rhs) noexcept
    : pReader_(std::exchange(rhs.pReader_, nullptr)),
      currentAwaitable_(std::exchange(rhs.currentAwaitable_, nullptr)),
      refCount_(rhs.refCount_),
      err_(std::move(rhs.err_)) {}

STDMETHODIMP SampleCallback::OnReadSample(HRESULT hr, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp,
                                          IMFSample* pSample) {
    if (FAILED(hr)) {
        err_ = {hr, "OnReadSample failed"};
        return S_OK;
    }
    if (pSample == nullptr) {
        _sample();
        return S_OK;
    }

    pSample->AddRef();
    currentAwaitable_->setPSample(pSample);
    currentAwaitable_->resume();

    return S_OK;
}

}  // namespace tcap::mf
