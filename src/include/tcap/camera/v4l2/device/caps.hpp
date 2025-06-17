#pragma once

#include <expected>

#include <linux/videodev2.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::v4l2 {

class DeviceCapsBox {
    DeviceCapsBox(uint32_t caps) noexcept;

public:
    [[nodiscard]] TCAP_API static std::expected<DeviceCapsBox, Error> create(const DeviceBox& deviceBox) noexcept;

    [[nodiscard]] TCAP_API bool supportVideoCapture() const noexcept { return caps_ & V4L2_CAP_VIDEO_CAPTURE; }

private:
    uint32_t caps_;
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/device/caps.cpp"
#endif
