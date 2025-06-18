#pragma once

#include <expected>
#include <vector>

#include "tcap/camera/v4l2/buffer/mmap.hpp"
#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/camera/v4l2/sample/mmap.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::v4l2 {

class QueueMMapBox {
    QueueMMapBox(std::shared_ptr<DeviceBox>&& pDeviceBox,
                 std::vector<std::shared_ptr<BufferViewMMap>>&& bufferViews) noexcept;

    [[nodiscard]] static std::expected<void, Error> pushAllBuffersHelper(DeviceBox& deviceBox,
                                                                         int bufferCount) noexcept;

public:
    QueueMMapBox(const QueueMMapBox&) = delete;
    QueueMMapBox& operator=(const QueueMMapBox&) = delete;
    TCAP_API QueueMMapBox(QueueMMapBox&&) noexcept = default;
    TCAP_API QueueMMapBox& operator=(QueueMMapBox&&) noexcept = default;
    TCAP_API ~QueueMMapBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<QueueMMapBox, Error> create(std::shared_ptr<DeviceBox> pDeviceBox,
                                                                            int bufferCount = 3) noexcept;

    [[nodiscard]] TCAP_API std::expected<void, Error> turnOnStream() noexcept;
    [[nodiscard]] TCAP_API std::expected<void, Error> turnOffStream() noexcept;
    [[nodiscard]] TCAP_API std::expected<void, Error> pushBuffer(SampleMMap&& sample) noexcept;
    [[nodiscard]] TCAP_API std::expected<SampleMMap, Error> popBuffer() noexcept;

private:
    std::shared_ptr<DeviceBox> pDeviceBox_;
    std::vector<std::shared_ptr<BufferViewMMap>> pBufferViews_;
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/queue/mmap.cpp"
#endif
