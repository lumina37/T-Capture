#pragma once

#include <expected>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::v4l2 {

class QueueBox {
    TCAP_API QueueBox() noexcept;

public:
    [[nodiscard]] TCAP_API static std::expected<QueueBox, Error> create(const DeviceBox& deviceBox,
                                                                        int bufferCount = 3) noexcept;

private:
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/queue.cpp"
#endif
