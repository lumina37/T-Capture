#include <mfidl.h>
#include <mfobjects.h>

#include "tcap/camera/mf/sample.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::mf {

SampleBox::SampleBox(IMFSample* pSample, const DWORD streamFlags, const LONGLONG timestamp) noexcept
    : pSample_(pSample), streamFlags_(streamFlags), timestamp_(timestamp) {}

std::expected<SampleBox, Error> SampleBox::create(IMFSample* pSample, const DWORD streamFlags,
                                                  const LONGLONG timestamp) noexcept {
    if (pSample != nullptr) {
        pSample->AddRef();
    }
    return SampleBox{pSample, streamFlags, timestamp};
}

SampleBox::SampleBox(SampleBox&& rhs) noexcept
    : pSample_(std::exchange(rhs.pSample_, nullptr)), streamFlags_(rhs.streamFlags_), timestamp_(rhs.timestamp_) {}

SampleBox& SampleBox::operator=(SampleBox&& rhs) noexcept {
    pSample_ = std::exchange(rhs.pSample_, nullptr);
    streamFlags_ = rhs.streamFlags_;
    timestamp_ = rhs.timestamp_;
    return *this;
}

SampleBox::~SampleBox() noexcept {
    if (pSample_ == nullptr) return;
    pSample_->Release();
    pSample_ = nullptr;
}

}  // namespace tcap::mf
