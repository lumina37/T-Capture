#include <expected>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format/active.hpp"
#endif

namespace tcap::v4l2 {

ActiveFormatBox::ActiveFormatBox(const v4l2_format& format) noexcept : format_(format) {}

ActiveFormatBox::ActiveFormatBox() noexcept : format_() {}

std::expected<ActiveFormatBox, Error> ActiveFormatBox::create(const DeviceBox& deviceBox) noexcept {
    const int fd = deviceBox.getFd();

    v4l2_format format;
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    const int ret = ioctl(fd, VIDIOC_G_FMT, &format);
    if (ret != 0) {
        return std::unexpected{Error{errno, "failed to get format"}};
    }

    return ActiveFormatBox{format};
}

std::expected<void, Error> ActiveFormatBox::apply(DeviceBox& deviceBox) const noexcept {
    const int fd = deviceBox.getFd();

    const int ret = ioctl(fd, VIDIOC_S_FMT, &format_);
    if (ret != 0) {
        return std::unexpected{Error{errno, "failed to set format"}};
    }

    return {};
}

}  // namespace tcap::v4l2
