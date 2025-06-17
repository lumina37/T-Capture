#pragma once

#include <expected>

#include <linux/videodev2.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::v4l2 {

class ActiveFormatBox {
    ActiveFormatBox(const v4l2_format& format) noexcept;

public:
    TCAP_API ActiveFormatBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<ActiveFormatBox, Error> create(const DeviceBox& deviceBox) noexcept;

    [[nodiscard]] TCAP_API int getWidth() const noexcept { return (int)format_.fmt.pix.width; }
    [[nodiscard]] TCAP_API int getHeight() const noexcept { return (int)format_.fmt.pix.height; }
    [[nodiscard]] TCAP_API uint32_t getFormat() const noexcept { return format_.fmt.pix.pixelformat; }
    [[nodiscard]] TCAP_API v4l2_colorspace getColorspace() const noexcept {
        return (v4l2_colorspace)format_.fmt.pix.colorspace;
    }

    TCAP_API void setWidth(int width) { format_.fmt.pix.width = width; }
    TCAP_API void setHeight(int height) { format_.fmt.pix.height = height; }
    TCAP_API void setFormat(uint32_t format) { format_.fmt.pix.pixelformat = format; }
    TCAP_API void setColorspace(v4l2_colorspace colorspace) { format_.fmt.pix.colorspace = colorspace; }

    [[nodiscard]] TCAP_API std::expected<void, Error> apply(DeviceBox& deviceBox) const noexcept;

private:
    v4l2_format format_;
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format/active.cpp"
#endif
