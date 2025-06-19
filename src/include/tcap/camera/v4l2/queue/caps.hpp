#pragma once

#include <expected>

#include <linux/videodev2.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::v4l2 {

class QueueCaps {
    QueueCaps(uint32_t caps) noexcept;

public:
    [[nodiscard]] TCAP_API static std::expected<QueueCaps, Error> create(const DeviceBox& deviceBox) noexcept;

    [[nodiscard]] TCAP_API bool supportMMapCacheHints() const noexcept {
        return caps_ & V4L2_BUF_CAP_SUPPORTS_MMAP_CACHE_HINTS;
    }
    [[nodiscard]] TCAP_API bool supportDMABuf() const noexcept { return caps_ & V4L2_BUF_CAP_SUPPORTS_DMABUF; }

private:
    uint32_t caps_;
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/queue/caps.cpp"
#endif
