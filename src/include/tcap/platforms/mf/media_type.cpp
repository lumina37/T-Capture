#include <mfapi.h>
#include <mfidl.h>

#include "tcap/helper/error.hpp"
#include "tcap/platforms/mf/source.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/media_type.hpp"
#endif

namespace tcap::mf {

MediaTypeBox::MediaTypeBox(GUID subTypeGuid, int width, int height, int fpsNumerator, int fpsDenominator) noexcept
    : subTypeGuid_(subTypeGuid),
      subType_(mapGuidToStreamSubType(subTypeGuid)),
      width_(width),
      height_(height),
      fpsNumerator_(fpsNumerator),
      fpsDenominator_(fpsDenominator),
      approxFps_((float)fpsNumerator / (float)fpsDenominator) {}

std::expected<MediaTypeBox, Error> MediaTypeBox::create(IMFMediaType* pMediaType) noexcept {
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

    return MediaTypeBox{subTypeGuid, (int)width, (int)height, (int)fpsNumerator, (int)fpsDenominator};
}

}  // namespace tcap::mf
