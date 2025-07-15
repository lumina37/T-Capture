#pragma once

#include <expected>

#include <mfobjects.h>

#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"
#include "tcap/utils/fourcc.hpp"

namespace tcap::mf {

class MediaTypeBox {
    MediaTypeBox(IMFMediaType* pMediaType, GUID subTypeGuid, int width, int height, int fpsNumerator,
                 int fpsDenominator) noexcept;

public:
    MediaTypeBox(const MediaTypeBox&) = delete;
    MediaTypeBox& operator=(const MediaTypeBox&) = delete;
    TCAP_API MediaTypeBox(MediaTypeBox&& rhs) noexcept;
    TCAP_API MediaTypeBox& operator=(MediaTypeBox&& rhs) noexcept;
    TCAP_API ~MediaTypeBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<MediaTypeBox, Error> create(IMFMediaType* pMediaType) noexcept;

    [[nodiscard]] TCAP_API IMFMediaType* getPMediaType() const noexcept { return pMediaType_; }
    [[nodiscard]] TCAP_API GUID getSubType() const noexcept { return subTypeGuid_; }
    [[nodiscard]] TCAP_API FourCC getSubTypeFourCC() const noexcept { return FourCC{subTypeGuid_.Data1}; }
    [[nodiscard]] TCAP_API int getWidth() const noexcept { return width_; }
    [[nodiscard]] TCAP_API int getHeight() const noexcept { return height_; }
    [[nodiscard]] TCAP_API float getApproxFps() const noexcept { return (float)fpsNumerator_ / (float)fpsDenominator_; }

private:
    IMFMediaType* pMediaType_;
    GUID subTypeGuid_;
    int width_;
    int height_;
    int fpsNumerator_;
    int fpsDenominator_;
};

}  // namespace tcap::mf
