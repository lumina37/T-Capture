#include <expected>
#include <vector>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format/resolution.hpp"
#endif

namespace tcap::v4l2 {

constexpr ResolutionBox::ResolutionBox(const int width, const int height) noexcept : width_(width), height_(height) {}

std::expected<std::vector<ResolutionBox>, Error> ResolutionBox::createBoxes(const DeviceBox& deviceBox,
                                                                            const uint32_t format) noexcept {
    const int fd = deviceBox.getFd();

    v4l2_frmsizeenum frmSize{};
    frmSize.index = 0;
    frmSize.pixel_format = format;

    std::vector<ResolutionBox> resolutionBoxes;
    for (; ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmSize) == 0; frmSize.index++) {
        if (frmSize.type != V4L2_FRMSIZE_TYPE_DISCRETE) continue;
        resolutionBoxes.emplace_back(frmSize.discrete.width, frmSize.discrete.height);
    }

    return resolutionBoxes;
}

}  // namespace tcap::v4l2
