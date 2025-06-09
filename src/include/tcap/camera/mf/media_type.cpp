#include <atlbase.h>
#include <mfapi.h>
#include <mfidl.h>

#include "tcap/camera/mf/source.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/media_type.hpp"
#endif

namespace tcap::mf {

StreamSubType mapGuidToStreamSubType(const GUID& guid) {
    if (guid == MFVideoFormat_I420) return StreamSubType::eI420;
    if (guid == MFVideoFormat_NV12) return StreamSubType::eNV12;
    return StreamSubType::eUnknown;
}

MediaTypeBox::MediaTypeBox(CComPtr<IMFMediaType>&& pMediaType, GUID subTypeGuid, int width, int height,
                           int fpsNumerator, int fpsDenominator) noexcept
    : pMediaType_(std::move(pMediaType)),
      subTypeGuid_(subTypeGuid),
      subType_(mapGuidToStreamSubType(subTypeGuid)),
      width_(width),
      height_(height),
      fpsNumerator_(fpsNumerator),
      fpsDenominator_(fpsDenominator),
      approxFps_((float)fpsNumerator / (float)fpsDenominator) {}

std::expected<MediaTypeBox, Error> MediaTypeBox::create(CComPtr<IMFMediaType>&& pMediaType) noexcept {
    HRESULT hr;

    GUID subTypeGuid;
    hr = pMediaType->GetGUID(MF_MT_SUBTYPE, &subTypeGuid);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "get subType failed"}};
    }

    UINT32 width, height;
    hr = MFGetAttributeSize(pMediaType, MF_MT_FRAME_SIZE, &width, &height);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "get size failed"}};
    }

    UINT32 fpsNumerator, fpsDenominator;
    hr = MFGetAttributeRatio(pMediaType, MF_MT_FRAME_RATE, &fpsNumerator, &fpsDenominator);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "get fps failed"}};
    }
    if (fpsDenominator == 0) {
        return std::unexpected{Error{hr, "fpsDenominator is 0"}};
    }

    return MediaTypeBox{std::move(pMediaType), subTypeGuid,       (int)width,
                        (int)height,           (int)fpsNumerator, (int)fpsDenominator};
}

}  // namespace tcap::mf
