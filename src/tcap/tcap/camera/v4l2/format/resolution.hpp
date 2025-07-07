#pragma once

#include <expected>
#include <vector>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::v4l2 {

class ResolutionBox {
public:
    TCAP_API constexpr ResolutionBox(int width, int height) noexcept;

    [[nodiscard]] TCAP_API static std::expected<std::vector<ResolutionBox>, Error> createBoxes(
        const DeviceBox& deviceBox, uint32_t format) noexcept;

    [[nodiscard]] TCAP_API constexpr int getWidth() const noexcept { return width_; }
    [[nodiscard]] TCAP_API constexpr int getHeight() const noexcept { return height_; }

private:
    int width_;
    int height_;
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format/resolution.cpp"
#endif
