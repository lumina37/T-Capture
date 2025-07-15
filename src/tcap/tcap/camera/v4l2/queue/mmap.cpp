#include <expected>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/camera/v4l2/queue/mmap.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::v4l2 {

QueueMMapBox::QueueMMapBox(DeviceBox&& deviceBox, std::vector<std::shared_ptr<BufferViewMMap>>&& bufferViews) noexcept
    : deviceBox_(std::move(deviceBox)), pBufferViews_(std::move(bufferViews)), isStreaming_(false) {}

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
            return std::unexpected{Error{ECate::eV4L2, errno}};
        }
    }

    return {};
}

QueueMMapBox::~QueueMMapBox() noexcept {
    const int fd = deviceBox_.getFd();
    if (fd < 0) return;

    if (isStreaming_) {
        turnOffStream().error();  // ignore exception
    }

    v4l2_requestbuffers bufferRequest{};
    bufferRequest.count = 0;
    bufferRequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferRequest.memory = V4L2_MEMORY_MMAP;

    ioctl(fd, VIDIOC_REQBUFS, &bufferRequest);
}

std::expected<QueueMMapBox, Error> QueueMMapBox::create(DeviceBox&& deviceBox, const int bufferCount) noexcept {
    if (bufferCount < 3) {
        return std::unexpected{Error{ECate::eTCap, ECode::eUnexValue, "bufferCount must be >= 3"}};
    }

    const int fd = deviceBox.getFd();

    v4l2_requestbuffers bufferRequest{};
    bufferRequest.count = bufferCount;
    bufferRequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferRequest.memory = V4L2_MEMORY_MMAP;

    const int ret = ioctl(fd, VIDIOC_REQBUFS, &bufferRequest);
    if (ret != 0) {
        return std::unexpected{Error{ECate::eV4L2, errno}};
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
            return std::unexpected{Error{ECate::eV4L2, errno}};
        }

        auto bufferViewRes = BufferViewMMap::create(deviceBox, bufferInfo);
        if (!bufferViewRes) return std::unexpected{std::move(bufferViewRes.error())};
        auto pBufferView = std::make_shared<BufferViewMMap>(std::move(bufferViewRes.value()));
        bufferViews.push_back(std::move(pBufferView));
    }

    auto pushBuffersRes = pushAllBuffersHelper(deviceBox, bufferCount);
    if (!pushBuffersRes) return std::unexpected{std::move(pushBuffersRes.error())};

    return QueueMMapBox{std::move(deviceBox), std::move(bufferViews)};
}

std::expected<void, Error> QueueMMapBox::turnOnStream() noexcept {
    const int fd = deviceBox_.getFd();

    v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    const int ret = ioctl(fd, VIDIOC_STREAMON, &type);
    if (ret != 0) {
        return std::unexpected{Error{ECate::eV4L2, errno}};
    }

    isStreaming_ = true;

    return {};
}

std::expected<void, Error> QueueMMapBox::turnOffStream() noexcept {
    const int fd = deviceBox_.getFd();

    v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    const int ret = ioctl(fd, VIDIOC_STREAMOFF, &type);
    if (ret != 0) {
        return std::unexpected{Error{ECate::eV4L2, errno}};
    }

    isStreaming_ = false;

    return {};
}

std::expected<void, Error> QueueMMapBox::pushBuffer(std::weak_ptr<BufferViewMMap>&& pBufferView) noexcept {
    const int fd = deviceBox_.getFd();

    v4l2_buffer bufferInfo{};
    bufferInfo.index = pBufferView.lock()->getIndex();
    bufferInfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferInfo.memory = V4L2_MEMORY_MMAP;

    const int ret = ioctl(fd, VIDIOC_QBUF, &bufferInfo);
    if (ret != 0) {
        return std::unexpected{Error{ECate::eV4L2, errno}};
    }

    return {};
}

std::expected<SampleMMap, Error> QueueMMapBox::popBuffer() noexcept {
    const int fd = deviceBox_.getFd();

    v4l2_buffer bufferInfo{};
    bufferInfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferInfo.memory = V4L2_MEMORY_MMAP;

    const int ret = ioctl(fd, VIDIOC_DQBUF, &bufferInfo);
    if (ret != 0) {
        return std::unexpected{Error{ECate::eV4L2, errno}};
    }

    auto pBuffer = std::weak_ptr{pBufferViews_[bufferInfo.index]};
    const timeval timestamp = bufferInfo.timestamp;
    const uint64_t timestampMs = timestamp.tv_sec * 1000ull + timestamp.tv_usec;

    return SampleMMap{std::move(pBuffer), timestampMs};
}

}  // namespace tcap::v4l2
