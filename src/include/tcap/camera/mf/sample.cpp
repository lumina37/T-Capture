#include <mfidl.h>
#include <mfobjects.h>

#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/sample.hpp"
#endif

namespace tcap::mf {

SampleBox::SampleBox(IMFSample* pSample, const DWORD streamIndex, const LONGLONG timestamp) noexcept
    : pSample_(pSample), streamIndex_(streamIndex), timestamp_(timestamp) {}

std::expected<SampleBox, Error> SampleBox::create(IMFSample* pSample, const DWORD streamIndex,
                                                  const LONGLONG timestamp) noexcept {
    pSample->AddRef();
    return SampleBox{pSample, streamIndex, timestamp};
}

SampleBox::SampleBox(SampleBox&& rhs) noexcept
    : pSample_(std::exchange(rhs.pSample_, nullptr)), streamIndex_(rhs.streamIndex_), timestamp_(rhs.timestamp_) {}

SampleBox& SampleBox::operator=(SampleBox&& rhs) noexcept {
    pSample_ = std::exchange(rhs.pSample_, nullptr);
    streamIndex_ = rhs.streamIndex_;
    timestamp_ = rhs.timestamp_;
    return *this;
}

SampleBox::~SampleBox() noexcept {
    if (pSample_ == nullptr) return;
    pSample_->Release();
    pSample_ = nullptr;
}

}  // namespace tcap::mf
