#pragma once

#include <expected>

#include <linux/videodev2.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::v4l2 {

class DeviceCaps {
    constexpr DeviceCaps(uint32_t caps) noexcept;

public:
    [[nodiscard]] TCAP_API static std::expected<DeviceCaps, Error> create(const DeviceBox& deviceBox) noexcept;

    [[nodiscard]] TCAP_API constexpr bool supportVideoCapture() const noexcept {
        return caps_ & V4L2_CAP_VIDEO_CAPTURE;
    }

private:
    uint32_t caps_;
};

}  // namespace tcap::v4l2
