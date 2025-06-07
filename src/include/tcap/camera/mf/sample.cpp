#include <mfidl.h>
#include <mfobjects.h>

#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/sample.hpp"
#endif

namespace tcap::mf {

SampleBox::SampleBox(IMFSample* pSample) noexcept : pSample_(pSample) {}

SampleBox::SampleBox(SampleBox&& rhs) noexcept : pSample_(std::exchange(rhs.pSample_, nullptr)) {}

SampleBox::~SampleBox() noexcept {
    if (pSample_ == nullptr) return;
    pSample_->Release();
    pSample_ = nullptr;
}

std::expected<SampleBox, Error> SampleBox::create(IMFSample* pSample) noexcept { return SampleBox{pSample}; }

}  // namespace tcap::mf
