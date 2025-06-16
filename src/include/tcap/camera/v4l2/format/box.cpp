#include <expected>
#include <utility>
#include <vector>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/camera/v4l2/format/resolution.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format/box.hpp"
#endif

namespace tcap::v4l2 {

FormatBox::FormatBox(const uint32_t format, std::vector<ResolutionBox>&& resolutionBoxes) noexcept
    : format_(format), resolutionBoxes_(std::move(resolutionBoxes)) {}

FormatBox& FormatBox::operator=(FormatBox&& rhs) noexcept {
    format_ = rhs.format_;
    resolutionBoxes_ = std::move(rhs.resolutionBoxes_);
    return *this;
}

FormatBox::FormatBox(FormatBox&& rhs) noexcept
    : format_(rhs.format_), resolutionBoxes_(std::move(rhs.resolutionBoxes_)) {}

std::expected<FormatBox, Error> FormatBox::create(const DeviceBox& deviceBox, const v4l2_fmtdesc& fmtDesc) noexcept {
    const int fd = deviceBox.getFd();

    v4l2_frmsizeenum frmSize;
    frmSize.index = 0;
    frmSize.pixel_format = fmtDesc.pixelformat;

    std::vector<ResolutionBox> resolutionBoxes;
    for (; ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmSize) == 0; frmSize.index++) {
        if (frmSize.type != V4L2_FRMSIZE_TYPE_DISCRETE) continue;

        auto resolutionBoxRes = ResolutionBox::create(deviceBox, fmtDesc, frmSize);
        if (!resolutionBoxRes) return std::unexpected{std::move(resolutionBoxRes.error())};
        resolutionBoxes.push_back(std::move(resolutionBoxRes.value()));
    }

    return FormatBox{fmtDesc.pixelformat, std::move(resolutionBoxes)};
}

}  // namespace tcap::v4l2
