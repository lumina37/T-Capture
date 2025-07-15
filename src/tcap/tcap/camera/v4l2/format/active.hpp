#pragma once

#include <expected>

#include <linux/videodev2.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::v4l2 {

class FormatActiveBox {
    constexpr FormatActiveBox(const v4l2_format& format) noexcept;

public:
    TCAP_API constexpr FormatActiveBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<FormatActiveBox, Error> create(const DeviceBox& deviceBox) noexcept;

    [[nodiscard]] TCAP_API constexpr int getWidth() const noexcept { return (int)format_.fmt.pix.width; }
    [[nodiscard]] TCAP_API constexpr int getHeight() const noexcept { return (int)format_.fmt.pix.height; }
    [[nodiscard]] TCAP_API constexpr uint32_t getFormat() const noexcept { return format_.fmt.pix.pixelformat; }
    [[nodiscard]] TCAP_API constexpr v4l2_colorspace getColorspace() const noexcept {
        return (v4l2_colorspace)format_.fmt.pix.colorspace;
    }

    TCAP_API constexpr void setWidth(const int width) noexcept { format_.fmt.pix.width = width; }
    TCAP_API constexpr void setHeight(const int height) noexcept { format_.fmt.pix.height = height; }
    TCAP_API constexpr void setFormat(const uint32_t format) noexcept { format_.fmt.pix.pixelformat = format; }
    TCAP_API constexpr void setColorspace(const v4l2_colorspace colorspace) noexcept {
        format_.fmt.pix.colorspace = colorspace;
    }

    [[nodiscard]] TCAP_API std::expected<void, Error> apply(DeviceBox& deviceBox) const noexcept;

private:
    v4l2_format format_;
};

}  // namespace tcap::v4l2
