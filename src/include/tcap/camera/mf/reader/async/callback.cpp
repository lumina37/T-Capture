#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader/async/callback.hpp"
#endif

namespace tcap::mf {

SampleCallback::SampleCallback() noexcept : pReader_(nullptr), currentAwaitable_(nullptr) {}

SampleCallback::SampleCallback(SampleCallback&& rhs) noexcept
    : pReader_(std::exchange(rhs.pReader_, nullptr)),
      currentAwaitable_(std::exchange(rhs.currentAwaitable_, nullptr)) {}

SampleCallback& SampleCallback::operator=(SampleCallback&& rhs) noexcept {
    pReader_ = std::exchange(rhs.pReader_, nullptr);
    currentAwaitable_ = std::exchange(rhs.currentAwaitable_, nullptr);
    return *this;
}

STDMETHODIMP SampleCallback::OnReadSample(HRESULT hr, [[maybe_unused]] DWORD streamIndex, DWORD streamFlags,
                                          LONGLONG timestamp, IMFSample* pSample) noexcept {
    if (FAILED(hr)) {
        {
            auto lock = currentAwaitable_->autoLock();
            currentAwaitable_->setSampleBoxRes(std::unexpected{Error{hr, "OnReadSample failed"}});
        }
        currentAwaitable_->resume();
        return S_OK;
    }

    {
        auto lock = currentAwaitable_->autoLock();
        if (pSample == nullptr) {
            currentAwaitable_->setSampleBoxRes(std::unexpected{Error{-1, "pSample is nullptr"}});
        } else {
            SampleBox sampleBox = SampleBox::create(pSample, streamFlags, timestamp).value();
            currentAwaitable_->setSampleBoxRes(std::move(sampleBox));
        }
    }

    currentAwaitable_->resume();  // goto `SampleAwaitable::await_resume`
    return S_OK;
}

void SampleCallback::sampleNonBlock() noexcept {}

}  // namespace tcap::mf
