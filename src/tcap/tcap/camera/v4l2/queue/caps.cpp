﻿#include <expected>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/queue/caps.hpp"
#endif

namespace tcap::v4l2 {

constexpr QueueCaps::QueueCaps(const uint32_t caps) noexcept : caps_(caps) {}

std::expected<QueueCaps, Error> QueueCaps::create(const DeviceBox& deviceBox) noexcept {
    const int fd = deviceBox.getFd();

    v4l2_requestbuffers bufferRequest{};
    bufferRequest.count = 0;
    bufferRequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferRequest.memory = V4L2_MEMORY_MMAP;

    const int ret = ioctl(fd, VIDIOC_REQBUFS, &bufferRequest);
    if (ret != 0) {
        return std::unexpected{Error{ECate::eV4L2, errno}};
    }

    const uint32_t caps = bufferRequest.capabilities;
    if (caps == 0) {
        return std::unexpected{Error{ECate::eTCap, ECode::eNoSupport}};
    }

    return QueueCaps{caps};
}

}  // namespace tcap::v4l2
