#pragma once

#include <expected>
#include <vector>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::v4l2 {

class ResolutionBox {
public:
    ResolutionBox(uint32_t width, uint32_t height) noexcept;

    [[nodiscard]] TCAP_API static std::expected<std::vector<ResolutionBox>, Error> createBoxes(
        const DeviceBox& deviceBox, uint32_t format) noexcept;

    [[nodiscard]] TCAP_API uint32_t getWidth() const noexcept { return width_; }
    [[nodiscard]] TCAP_API uint32_t getHeight() const noexcept { return height_; }

private:
    uint32_t width_;
    uint32_t height_;
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format/resolution.cpp"
#endif
