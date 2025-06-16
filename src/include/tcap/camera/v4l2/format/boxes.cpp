#include <expected>
#include <utility>
#include <vector>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/camera/v4l2/format/box.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format/boxes.hpp"
#endif

namespace tcap::v4l2 {

FormatBoxes::FormatBoxes(std::vector<FormatBox>&& formatBoxes) noexcept : formatBoxes_(std::move(formatBoxes)) {}

FormatBoxes& FormatBoxes::operator=(FormatBoxes&& rhs) noexcept {
    formatBoxes_ = std::move(rhs.formatBoxes_);
    return *this;
}

FormatBoxes::FormatBoxes(FormatBoxes&& rhs) noexcept : formatBoxes_(std::move(rhs.formatBoxes_)) {}

std::expected<FormatBoxes, Error> FormatBoxes::create(const DeviceBox& deviceBox) noexcept {
    const int fd = deviceBox.getFd();

    v4l2_fmtdesc fmtDesc;
    fmtDesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmtDesc.index = 0;

    std::vector<FormatBox> formatBoxes;
    for (; ioctl(fd, VIDIOC_ENUM_FMT, &fmtDesc) == 0; fmtDesc.index++) {
        auto formatBoxRes = FormatBox::create(deviceBox, fmtDesc);
        if (!formatBoxRes) return std::unexpected{std::move(formatBoxRes.error())};
        formatBoxes.push_back(std::move(formatBoxRes.value()));
    }

    return FormatBoxes{std::move(formatBoxes)};
}

}  // namespace tcap::v4l2
