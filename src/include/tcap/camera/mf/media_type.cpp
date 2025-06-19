#include <mfapi.h>
#include <mfidl.h>

#include "tcap/camera/mf/source.hpp"
#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/media_type.hpp"
#endif

namespace tcap::mf {

MediaTypeBox::MediaTypeBox(IMFMediaType* pMediaType, GUID subTypeGuid, int width, int height, int fpsNumerator,
                           int fpsDenominator) noexcept
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
        return std::unexpected{Error{ECate::eTCap, ECode::eUnexValue, "fpsDenominator is 0"}};
    }

    return MediaTypeBox{std::move(pMediaType), subTypeGuid,       (int)width,
                        (int)height,           (int)fpsNumerator, (int)fpsDenominator};
}

}  // namespace tcap::mf
