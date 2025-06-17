#pragma once

#include <expected>

#include <linux/videodev2.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::v4l2 {

class BufferViewMMap {
    BufferViewMMap(void* pBuffer, uint32_t length, int index) noexcept;

public:
    BufferViewMMap& operator=(const BufferViewMMap&) = delete;
    BufferViewMMap(const BufferViewMMap&) = delete;
    TCAP_API BufferViewMMap(BufferViewMMap&& rhs) noexcept;
    TCAP_API BufferViewMMap& operator=(BufferViewMMap&& rhs) noexcept;
    TCAP_API ~BufferViewMMap() noexcept;

    [[nodiscard]] TCAP_API static std::expected<BufferViewMMap, Error> create(DeviceBox& deviceBox,
                                                                              const v4l2_buffer& bufferInfo) noexcept;

    [[nodiscard]] TCAP_API int getIndex() const noexcept { return index_; }

private:
    void* pBuffer_;
    uint32_t length_;
    int index_;
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/buffer/mmap.cpp"
#endif
