#pragma once

#include <expected>
#include <vector>

#include "tcap/camera/v4l2/buffer/mmap.hpp"
#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::v4l2 {

class QueueMMapBox {
    QueueMMapBox() noexcept;

public:
    [[nodiscard]] TCAP_API static std::expected<QueueMMapBox, Error> create(DeviceBox& deviceBox,
                                                                            int bufferCount = 3) noexcept;

private:
    std::vector<BufferViewMMap> bufferViews_;
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/queue/mmap.cpp"
#endif
