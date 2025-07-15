#include <mfapi.h>
#include <mfidl.h>

#include "tcap/camera/mf/media_type.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::mf {

MediaTypeBox::MediaTypeBox(IMFMediaType* pMediaType, const GUID subTypeGuid, const int width, const int height,
                           const int fpsNumerator, const int fpsDenominator) noexcept
    : pMediaType_(pMediaType),
      subTypeGuid_(subTypeGuid),
      width_(width),
      height_(height),
      fpsNumerator_(fpsNumerator),
      fpsDenominator_(fpsDenominator) {}

MediaTypeBox::MediaTypeBox(MediaTypeBox&& rhs) noexcept
    : pMediaType_(std::exchange(rhs.pMediaType_, nullptr)),
      subTypeGuid_(rhs.subTypeGuid_),
      width_(rhs.width_),
      height_(rhs.height_),
      fpsNumerator_(rhs.fpsNumerator_),
      fpsDenominator_(rhs.fpsDenominator_) {}

MediaTypeBox& MediaTypeBox::operator=(MediaTypeBox&& rhs) noexcept {
    pMediaType_ = std::exchange(rhs.pMediaType_, nullptr);
    subTypeGuid_ = rhs.subTypeGuid_;
    width_ = rhs.width_;
    height_ = rhs.height_;
    fpsNumerator_ = rhs.fpsNumerator_;
    fpsDenominator_ = rhs.fpsDenominator_;
    return *this;
}

MediaTypeBox::~MediaTypeBox() noexcept {
    if (pMediaType_ == nullptr) return;
    pMediaType_->Release();
    pMediaType_ = nullptr;
}

std::expected<MediaTypeBox, Error> MediaTypeBox::create(IMFMediaType* pMediaType) noexcept {
    pMediaType->AddRef();

    GUID subTypeGuid;
    pMediaType->GetGUID(MF_MT_SUBTYPE, &subTypeGuid);

    UINT32 width, height;
    MFGetAttributeSize(pMediaType, MF_MT_FRAME_SIZE, &width, &height);

    UINT32 fpsNumerator, fpsDenominator;
    MFGetAttributeRatio(pMediaType, MF_MT_FRAME_RATE, &fpsNumerator, &fpsDenominator);
    if (fpsDenominator == 0) {
        pMediaType->Release();
        return std::unexpected{Error{ECate::eTCap, ECode::eUnexValue, "fpsDenominator is 0"}};
    }

    return MediaTypeBox{pMediaType, subTypeGuid, (int)width, (int)height, (int)fpsNumerator, (int)fpsDenominator};
}

}  // namespace tcap::mf
