#include <cstring>
#include <expected>
#include <utility>

#include <linux/videodev2.h>
#include <sys/mman.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/buffer/mmap.hpp"
#endif

namespace tcap::v4l2 {

BufferViewMMap::BufferViewMMap(void* pBuffer, const uint32_t length, const int index) noexcept
    : pBuffer_(pBuffer), length_(length), index_(index) {}

BufferViewMMap::BufferViewMMap(BufferViewMMap&& rhs) noexcept
    : pBuffer_(std::exchange(rhs.pBuffer_, nullptr)), length_(rhs.length_), index_(rhs.index_) {}

BufferViewMMap& BufferViewMMap::operator=(BufferViewMMap&& rhs) noexcept {
    pBuffer_ = std::exchange(rhs.pBuffer_, nullptr);
    length_ = rhs.length_;
    index_ = rhs.index_;
    return *this;
}

BufferViewMMap::~BufferViewMMap() noexcept {
    if (pBuffer_ == nullptr) return;
    munmap(pBuffer_, length_);
    pBuffer_ = nullptr;
}

std::expected<BufferViewMMap, Error> BufferViewMMap::create(DeviceBox& deviceBox,
                                                            const v4l2_buffer& bufferInfo) noexcept {
    const int fd = deviceBox.getFd();

    const uint32_t length = bufferInfo.length;
    void* pData = mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, bufferInfo.m.offset);
    if (pData == nullptr) {
        return std::unexpected{Error{ECate::eV4L2, errno}};
    }

    return BufferViewMMap{pData, length, (int)bufferInfo.index};
}

void BufferViewMMap::copyTo(std::byte* pData) const noexcept { std::memcpy(pData, pBuffer_, length_); }

}  // namespace tcap::v4l2
