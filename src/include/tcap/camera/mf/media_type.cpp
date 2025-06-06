#include <mfapi.h>
#include <mfidl.h>

#include "tcap/camera/mf/source.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/media_type.hpp"
#endif

namespace tcap::mf {

MediaTypeBox::MediaTypeBox(int fpsNumerator, int fpsDenominator, float approxFps) noexcept
    : fpsNumerator_(fpsNumerator), fpsDenominator_(fpsDenominator), approxFps_(approxFps) {}

std::expected<MediaTypeBox, Error> MediaTypeBox::create(IMFMediaType* pMediaType) noexcept {
    HRESULT hr;

    UINT32 fpsNumerator;
    UINT32 fpsDenominator;
    hr = MFGetAttributeRatio(pMediaType, MF_MT_FRAME_RATE, &fpsNumerator, &fpsDenominator);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "MFGetAttributeRatio(pMediaType) failed"}};
    }
    if (fpsDenominator == 0) {
        return std::unexpected{Error{hr, "fpsDenominator is 0"}};
    }
    const float fps = (float)fpsNumerator / (float)fpsDenominator;

    return MediaTypeBox{(int)fpsNumerator, (int)fpsDenominator, fps};
}

}  // namespace tcap::mf
