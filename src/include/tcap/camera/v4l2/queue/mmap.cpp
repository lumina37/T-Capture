#include <expected>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/queue/mmap.hpp"
#endif

namespace tcap::v4l2 {

QueueMMapBox::QueueMMapBox(std::shared_ptr<DeviceBox>&& pDeviceBox,
                           std::vector<std::shared_ptr<BufferViewMMap>>&& bufferViews) noexcept
    : pDeviceBox_(std::move(pDeviceBox)), pBufferViews_(std::move(bufferViews)) {}

std::expected<void, Error> QueueMMapBox::pushAllBuffersHelper(DeviceBox& deviceBox, const int bufferCount) noexcept {
    const int fd = deviceBox.getFd();

    v4l2_buffer bufferInfo{};
    bufferInfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferInfo.memory = V4L2_MEMORY_MMAP;

    for (int index = 0; index < bufferCount; index++) {
        bufferInfo.index = index;
        bufferInfo.flags = 0;
        const int ret = ioctl(fd, VIDIOC_QBUF, &bufferInfo);
        if (ret != 0) {
            return std::unexpected{Error{errno, "failed to push buffer into queue"}};
        }
    }

    return {};
}

QueueMMapBox::~QueueMMapBox() noexcept {
    if (pDeviceBox_ == nullptr) return;

    const int fd = pDeviceBox_->getFd();

    v4l2_requestbuffers bufferRequest{};
    bufferRequest.count = 0;
    bufferRequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferRequest.memory = V4L2_MEMORY_MMAP;

    ioctl(fd, VIDIOC_REQBUFS, &bufferRequest);

    pDeviceBox_ = nullptr;
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

    std::vector<std::shared_ptr<BufferViewMMap>> bufferViews;
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
        auto pBufferView = std::make_shared<BufferViewMMap>(std::move(bufferViewRes.value()));
        bufferViews.push_back(std::move(pBufferView));
    }

    auto pushBuffersRes = pushAllBuffersHelper(*pDeviceBox, bufferCount);
    if (!pushBuffersRes) return std::unexpected{std::move(pushBuffersRes.error())};

    return QueueMMapBox{std::move(pDeviceBox), std::move(bufferViews)};
}

std::expected<void, Error> QueueMMapBox::turnOnStream() noexcept {
    const int fd = pDeviceBox_->getFd();

    v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    const int ret = ioctl(fd, VIDIOC_STREAMON, &type);
    if (ret != 0) {
        return std::unexpected{Error{errno, "failed to launch stream"}};
    }

    return {};
}

std::expected<void, Error> QueueMMapBox::turnOffStream() noexcept {
    const int fd = pDeviceBox_->getFd();

    v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    const int ret = ioctl(fd, VIDIOC_STREAMOFF, &type);
    if (ret != 0) {
        return std::unexpected{Error{errno, "failed to stop stream"}};
    }

    return {};
}

std::expected<void, Error> QueueMMapBox::pushBuffer(SampleMMap&& sample) noexcept {
    const int fd = pDeviceBox_->getFd();

    v4l2_buffer bufferInfo{};
    bufferInfo.index = sample.take().lock()->getIndex();
    bufferInfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferInfo.memory = V4L2_MEMORY_MMAP;

    const int ret = ioctl(fd, VIDIOC_QBUF, &bufferInfo);
    if (ret != 0) {
        return std::unexpected{Error{errno, "failed to push buffer into queue"}};
    }

    return {};
}

std::expected<SampleMMap, Error> QueueMMapBox::popBuffer() noexcept {
    const int fd = pDeviceBox_->getFd();

    v4l2_buffer bufferInfo{};
    bufferInfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferInfo.memory = V4L2_MEMORY_MMAP;

    const int ret = ioctl(fd, VIDIOC_DQBUF, &bufferInfo);
    if (ret != 0) {
        return std::unexpected{Error{errno, "failed to pop buffer from queue"}};
    }

    auto pBuffer = std::weak_ptr{pBufferViews_[bufferInfo.index]};
    const timeval timestamp = bufferInfo.timestamp;
    const uint64_t timestampNs = timestamp.tv_sec * 1000000000ull + timestamp.tv_usec * 1000000ull;

    return SampleMMap{std::move(pBuffer), timestampNs};
}

}  // namespace tcap::v4l2
