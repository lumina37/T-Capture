#include <expected>
#include <vector>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/camera/v4l2/format/fps.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::v4l2 {

constexpr FpsBox::FpsBox(const uint32_t numerator, const uint32_t denominator) noexcept
    : numerator_(numerator), denominator_(denominator) {}

std::expected<std::vector<FpsBox>, Error> FpsBox::createBoxes(const DeviceBox& deviceBox, const uint32_t format,
                                                              const uint32_t width, const uint32_t height) noexcept {
    const int fd = deviceBox.getFd();

    v4l2_frmivalenum frmIval{};
    frmIval.index = 0;
    frmIval.pixel_format = format;
    frmIval.width = width;
    frmIval.height = height;

    std::vector<FpsBox> fpsBoxes;
    for (; ioctl(fd, VIDIOC_ENUM_FRAMEINTERVALS, &frmIval) == 0; frmIval.index++) {
        if (frmIval.type != V4L2_FRMIVAL_TYPE_DISCRETE) continue;
        fpsBoxes.emplace_back(frmIval.discrete.numerator, frmIval.discrete.denominator);
    }

    return fpsBoxes;
}

std::expected<void, Error> FpsBox::apply(DeviceBox& deviceBox) const noexcept {
    const int fd = deviceBox.getFd();

    v4l2_streamparm streamParam{};
    streamParam.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    streamParam.parm.capture.timeperframe.numerator = numerator_;
    streamParam.parm.capture.timeperframe.denominator = denominator_;

    const int ret = ioctl(fd, VIDIOC_S_PARM, &streamParam);
    if (ret != 0) {
        return std::unexpected{Error{ECate::eV4L2, errno}};
    }

    return {};
}

}  // namespace tcap::v4l2
