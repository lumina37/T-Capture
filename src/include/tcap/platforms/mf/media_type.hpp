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

static StreamSubType mapGuidToStreamSubType(const GUID& guid) {
    if (guid == MFVideoFormat_I420) return StreamSubType::eI420;
    if (guid == MFVideoFormat_NV12) return StreamSubType::eNV12;
    return StreamSubType::eUnknown;
}

class MediaTypeBox {
    MediaTypeBox(GUID subTypeGuid, int width, int height, int fpsNumerator, int fpsDenominator) noexcept;

public:
    [[nodiscard]] TCAP_API static std::expected<MediaTypeBox, Error> create(IMFMediaType* pMediaType) noexcept;

    [[nodiscard]] TCAP_API GUID getSubTypeGuid() const noexcept { return subTypeGuid_; }
    [[nodiscard]] TCAP_API StreamSubType getSubType() const noexcept { return subType_; }
    [[nodiscard]] TCAP_API int getWidth() const noexcept { return width_; }
    [[nodiscard]] TCAP_API int getHeight() const noexcept { return height_; }
    [[nodiscard]] TCAP_API float getApproxFps() const noexcept { return approxFps_; }

private:
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
