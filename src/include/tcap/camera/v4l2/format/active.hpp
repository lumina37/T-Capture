#pragma once

#include <expected>

#include <linux/videodev2.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::v4l2 {

class ActiveFormatBox {
public:
    TCAP_API ActiveFormatBox(const v4l2_pix_format& format) noexcept;

    [[nodiscard]] TCAP_API static std::expected<ActiveFormatBox, Error> create(const DeviceBox& deviceBox) noexcept;

    [[nodiscard]] TCAP_API int getWidth() const noexcept { return (int)format_.width; }
    [[nodiscard]] TCAP_API int getHeight() const noexcept { return (int)format_.height; }
    [[nodiscard]] TCAP_API uint32_t getFormat() const noexcept { return format_.pixelformat; }
    [[nodiscard]] TCAP_API v4l2_colorspace getColorspace() const noexcept {
        return (v4l2_colorspace)format_.colorspace;
    }

private:
    v4l2_pix_format format_;
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format/active.cpp"
#endif
