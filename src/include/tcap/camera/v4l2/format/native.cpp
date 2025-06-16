#include <expected>
#include <vector>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format/native.hpp"
#endif

namespace tcap::v4l2 {

NativeFormatBox::NativeFormatBox(const uint32_t format) noexcept : format_(format) {}

std::expected<std::vector<NativeFormatBox>, Error> NativeFormatBox::createBoxes(const DeviceBox& deviceBox) noexcept {
    const int fd = deviceBox.getFd();

    v4l2_fmtdesc fmtDesc;
    fmtDesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmtDesc.index = 0;

    std::vector<NativeFormatBox> formatBoxes;
    for (; ioctl(fd, VIDIOC_ENUM_FMT, &fmtDesc) == 0; fmtDesc.index++) {
        formatBoxes.emplace_back(fmtDesc.pixelformat);
    }

    return formatBoxes;
}

}  // namespace tcap::v4l2
