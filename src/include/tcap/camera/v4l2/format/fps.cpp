#include <expected>
#include <vector>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format/fps.hpp"
#endif

namespace tcap::v4l2 {

FpsBox::FpsBox(const uint32_t numerator, const uint32_t denominator) noexcept
    : numerator_(numerator), denominator_(denominator) {}

std::expected<std::vector<FpsBox>, Error> FpsBox::createBoxes(const DeviceBox& deviceBox, uint32_t format,
                                                              uint32_t width, uint32_t height) noexcept {
    const int fd = deviceBox.getFd();

    v4l2_frmivalenum frmIval;
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

}  // namespace tcap::v4l2
