#include <expected>
#include <utility>
#include <vector>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format/resolution.hpp"
#endif

namespace tcap::v4l2 {

ResolutionBox::ResolutionBox(const uint32_t width, const uint32_t height, std::vector<Fps>&& fpss) noexcept
    : width_(width), height_(height), fpss_(std::move(fpss)) {}

ResolutionBox& ResolutionBox::operator=(ResolutionBox&& rhs) noexcept {
    width_ = rhs.width_;
    height_ = rhs.height_;
    fpss_ = std::move(rhs.fpss_);
    return *this;
}

ResolutionBox::ResolutionBox(ResolutionBox&& rhs) noexcept
    : width_(rhs.width_), height_(rhs.height_), fpss_(std::move(rhs.fpss_)) {}

std::expected<ResolutionBox, Error> ResolutionBox::create(const DeviceBox& deviceBox, const v4l2_fmtdesc& fmtDesc,
                                                          const v4l2_frmsizeenum& frmSize) noexcept {
    const int fd = deviceBox.getFd();

    v4l2_frmivalenum frmIval;
    frmIval.index = 0;
    frmIval.pixel_format = fmtDesc.pixelformat;
    frmIval.width = frmSize.discrete.width;
    frmIval.height = frmSize.discrete.height;

    std::vector<Fps> fpss;
    for (; ioctl(fd, VIDIOC_ENUM_FRAMEINTERVALS, &frmIval) == 0; frmIval.index++) {
        if (frmIval.type != V4L2_FRMIVAL_TYPE_DISCRETE) continue;
        fpss.emplace_back(frmIval.discrete.numerator, frmIval.discrete.denominator);
    }

    return ResolutionBox{frmSize.discrete.width, frmSize.discrete.height, std::move(fpss)};
}

}  // namespace tcap::v4l2
