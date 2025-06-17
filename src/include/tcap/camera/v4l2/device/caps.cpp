#include <expected>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/device/caps.hpp"
#endif

namespace tcap::v4l2 {

DeviceCapsBox::DeviceCapsBox(uint32_t caps) noexcept : caps_(caps) {}

std::expected<DeviceCapsBox, Error> DeviceCapsBox::create(const DeviceBox& deviceBox) noexcept {
    const int fd = deviceBox.getFd();

    v4l2_capability caps;

    const int ret = ioctl(fd, VIDIOC_QUERYCAP, &caps);
    if (ret != 0) {
        return std::unexpected{Error{errno, "failed to get caps"}};
    }

    const uint32_t capFlags = caps.capabilities;
    if ((capFlags & V4L2_CAP_DEVICE_CAPS) == 0) {
        return std::unexpected{Error{-1, "device caps is not supported"}};
    }

    return DeviceCapsBox{caps.device_caps};
}

}  // namespace tcap::v4l2
