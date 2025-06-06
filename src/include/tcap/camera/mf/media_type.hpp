#pragma once

#include <expected>

#include <mfidl.h>

#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class MediaTypeBox {
    MediaTypeBox(int fpsNumerator, int fpsDenominator, float approxFps) noexcept;

public:
    [[nodiscard]] TCAP_API static std::expected<MediaTypeBox, Error> create(IMFMediaType* pMediaType) noexcept;

    [[nodiscard]] TCAP_API float getApproxFps() const noexcept { return approxFps_; }

private:
    int fpsNumerator_;
    int fpsDenominator_;
    float approxFps_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/media_type.cpp"
#endif
