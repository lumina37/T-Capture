#include <expected>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/queue/mmap.hpp"
#endif

namespace tcap::v4l2 {

QueueMMapBox::QueueMMapBox(std::shared_ptr<DeviceBox>&& pDeviceBox, std::vector<BufferViewMMap>&& bufferViews) noexcept
    : pDeviceBox_(std::move(pDeviceBox)), bufferViews_(std::move(bufferViews)) {}

std::expected<void, Error> QueueMMapBox::pushAllBuffersHelper(DeviceBox& deviceBox,
                                                              std::vector<BufferViewMMap>& bufferViews) noexcept {
    const int fd = deviceBox.getFd();

    v4l2_buffer bufferInfo{};
    bufferInfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferInfo.memory = V4L2_MEMORY_MMAP;

    for (auto& bufferView : bufferViews) {
        bufferInfo.index = bufferView.getIndex();
        bufferInfo.flags = 0;
        const int ret = ioctl(fd, VIDIOC_QBUF, &bufferInfo);
        if (ret != 0) {
            return std::unexpected{Error{errno, "failed to push buffer into queue"}};
        }
    }

    return {};
}

std::expected<QueueMMapBox, Error> QueueMMapBox::create(std::shared_ptr<DeviceBox> pDeviceBox,
                                                        const int bufferCount) noexcept {
    if (bufferCount < 3) {
        return std::unexpected{Error{-1, "bufferCount must be >= 3"}};
    }

    const int fd = pDeviceBox->getFd();

    v4l2_requestbuffers bufferRequest{};
    bufferRequest.count = bufferCount;
    bufferRequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferRequest.memory = V4L2_MEMORY_MMAP;

    const int ret = ioctl(fd, VIDIOC_REQBUFS, &bufferRequest);
    if (ret != 0) {
        return std::unexpected{Error{errno, "failed to request buffers"}};
    }

    std::vector<BufferViewMMap> bufferViews;
    bufferViews.reserve(bufferCount);
    for (int bufferIndex = 0; bufferIndex < bufferCount; bufferIndex++) {
        v4l2_buffer bufferInfo{};
        bufferInfo.index = bufferIndex;
        bufferInfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        bufferInfo.memory = V4L2_MEMORY_MMAP;

        const int queryRet = ioctl(fd, VIDIOC_QUERYBUF, &bufferInfo);
        if (queryRet != 0) {
            return std::unexpected{Error{errno, "failed to query buffer info"}};
        }

        auto bufferViewRes = BufferViewMMap::create(*pDeviceBox, bufferInfo);
        if (!bufferViewRes) return std::unexpected{std::move(bufferViewRes.error())};
        bufferViews.push_back(std::move(bufferViewRes.value()));
    }

    auto pushBuffersRes = pushAllBuffersHelper(*pDeviceBox, bufferViews);
    if (!pushBuffersRes) return std::unexpected{std::move(pushBuffersRes.error())};

    return QueueMMapBox{std::move(pDeviceBox), std::move(bufferViews)};
}

std::expected<void, Error> QueueMMapBox::startStream() noexcept {
    const int fd = pDeviceBox_->getFd();

    v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    const int ret = ioctl(fd, VIDIOC_STREAMON, &type);
    if (ret != 0) {
        return std::unexpected{Error{errno, "failed to launch stream"}};
    }

    return {};
}

std::expected<void, Error> QueueMMapBox::stopStream() noexcept {
    const int fd = pDeviceBox_->getFd();

    v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    const int ret = ioctl(fd, VIDIOC_STREAMOFF, &type);
    if (ret != 0) {
        return std::unexpected{Error{errno, "failed to stop stream"}};
    }

    return {};
}

std::expected<void, Error> QueueMMapBox::pushBuffer(BufferViewMMap& bufferView) noexcept {
    const int fd = pDeviceBox_->getFd();

    v4l2_buffer bufferInfo{};
    bufferInfo.index = bufferView.getIndex();
    bufferInfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferInfo.memory = V4L2_MEMORY_MMAP;

    const int ret = ioctl(fd, VIDIOC_QBUF, &bufferInfo);
    if (ret != 0) {
        return std::unexpected{Error{errno, "failed to push buffer into queue"}};
    }

    return {};
}

std::expected<std::reference_wrapper<BufferViewMMap>, Error> QueueMMapBox::popBuffer() noexcept {
    const int fd = pDeviceBox_->getFd();

    v4l2_buffer bufferInfo{};
    bufferInfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferInfo.memory = V4L2_MEMORY_MMAP;

    const int ret = ioctl(fd, VIDIOC_DQBUF, &bufferInfo);
    if (ret != 0) {
        return std::unexpected{Error{errno, "failed to pop buffer from queue"}};
    }

    auto& bufferView = bufferViews_[bufferInfo.index];
    return bufferView;
}

}  // namespace tcap::v4l2
