#include <expected>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/device/caps.hpp"
#endif

namespace tcap::v4l2 {

DeviceCaps::DeviceCaps(uint32_t caps) noexcept : caps_(caps) {}

std::expected<DeviceCaps, Error> DeviceCaps::create(const DeviceBox& deviceBox) noexcept {
    const int fd = deviceBox.getFd();

    v4l2_capability caps;

    const int ret = ioctl(fd, VIDIOC_QUERYCAP, &caps);
    if (ret != 0) {
        return std::unexpected{Error{ECate::eV4L2, errno}};
    }

    const uint32_t capFlags = caps.capabilities;
    if ((capFlags & V4L2_CAP_DEVICE_CAPS) == 0) {
        return std::unexpected{Error{ECate::eTCap, ECode::eNoSupport}};
    }

    return DeviceCaps{caps.device_caps};
}

}  // namespace tcap::v4l2
