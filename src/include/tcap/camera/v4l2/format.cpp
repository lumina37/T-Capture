#include <expected>
#include <utility>
#include <vector>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format.hpp"
#endif

namespace tcap::v4l2 {

FormatBox::FormatBox(std::vector<v4l2_fmtdesc>&& fmtDescs) noexcept : fmtDescs_(std::move(fmtDescs)) {}

FormatBox& FormatBox::operator=(FormatBox&& rhs) noexcept {
    fmtDescs_ = std::move(rhs.fmtDescs_);
    return *this;
}

FormatBox::FormatBox(FormatBox&& rhs) noexcept : fmtDescs_(std::move(rhs.fmtDescs_)) {}

std::expected<FormatBox, Error> FormatBox::create(const DeviceBox& deviceBox) noexcept {
    const int fd = deviceBox.getFd();

    v4l2_fmtdesc fmtDesc;
    fmtDesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmtDesc.index = 0;

    std::vector<v4l2_fmtdesc> fmtDescs;
    while (true) {
        const int ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmtDesc);
        if (ret != 0) break;

        fmtDescs.push_back(fmtDesc);
        fmtDesc.index++;
    }

    return FormatBox{std::move(fmtDescs)};
}

}  // namespace tcap::v4l2
