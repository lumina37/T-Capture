#pragma once

#include <expected>

#include "mfobjects.h"

#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

enum class StreamSubType {
    eUnknown = 0,
    eI420,
    eNV12,
};

StreamSubType mapGuidToStreamSubType(const GUID& guid);

class MediaTypeBox {
    MediaTypeBox(IMFMediaType* pMediaType, GUID subTypeGuid, int width, int height, int fpsNumerator,
                 int fpsDenominator) noexcept;

public:
    TCAP_API MediaTypeBox(MediaTypeBox&& rhs) noexcept;
    TCAP_API ~MediaTypeBox() noexcept;

    // this will take the lifetime of the `IMFMediaType*`
    [[nodiscard]] TCAP_API static std::expected<MediaTypeBox, Error> create(IMFMediaType* pMediaType) noexcept;

    [[nodiscard]] TCAP_API GUID getSubTypeGuid() const noexcept { return subTypeGuid_; }
    [[nodiscard]] TCAP_API StreamSubType getSubType() const noexcept { return subType_; }
    [[nodiscard]] TCAP_API int getWidth() const noexcept { return width_; }
    [[nodiscard]] TCAP_API int getHeight() const noexcept { return height_; }
    [[nodiscard]] TCAP_API float getApproxFps() const noexcept { return approxFps_; }

private:
    IMFMediaType* pMediaType_;
    GUID subTypeGuid_;
    StreamSubType subType_;
    int width_;
    int height_;
    int fpsNumerator_;
    int fpsDenominator_;
    float approxFps_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/media_type.cpp"
#endif
