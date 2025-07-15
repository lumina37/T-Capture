#include <expected>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/camera/v4l2/format/active.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::v4l2 {

constexpr FormatActiveBox::FormatActiveBox(const v4l2_format& format) noexcept : format_(format) {
    format_.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
}

constexpr FormatActiveBox::FormatActiveBox() noexcept : format_() {}

std::expected<FormatActiveBox, Error> FormatActiveBox::create(const DeviceBox& deviceBox) noexcept {
    const int fd = deviceBox.getFd();

    v4l2_format format{};
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    const int ret = ioctl(fd, VIDIOC_G_FMT, &format);
    if (ret != 0) {
        return std::unexpected{Error{ECate::eV4L2, errno}};
    }

    return FormatActiveBox{format};
}

std::expected<void, Error> FormatActiveBox::apply(DeviceBox& deviceBox) const noexcept {
    const int fd = deviceBox.getFd();

    const int ret = ioctl(fd, VIDIOC_S_FMT, &format_);
    if (ret != 0) {
        return std::unexpected{Error{ECate::eV4L2, errno}};
    }

    return {};
}

}  // namespace tcap::v4l2
