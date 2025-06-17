#include <expected>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/queue/box.hpp"
#endif

namespace tcap::v4l2 {

QueueBox::QueueBox() noexcept {}

std::expected<QueueBox, Error> QueueBox::create(const DeviceBox& deviceBox, const int bufferCount) noexcept {
    const int fd = deviceBox.getFd();

    v4l2_requestbuffers bufferRequest;
    bufferRequest.count = bufferCount;
    bufferRequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferRequest.memory = V4L2_MEMORY_MMAP;

    const int ret = ioctl(fd, VIDIOC_REQBUFS, &bufferRequest);
    if (ret != 0) {
        return std::unexpected{Error{errno, "failed to request buffers"}};
    }

    return QueueBox{};
}

}  // namespace tcap::v4l2
