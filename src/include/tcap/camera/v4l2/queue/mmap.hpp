#pragma once

#include <expected>
#include <vector>

#include "tcap/camera/v4l2/buffer/mmap.hpp"
#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::v4l2 {

class QueueMMapBox {
    QueueMMapBox(std::shared_ptr<DeviceBox>&& pDeviceBox, std::vector<BufferViewMMap>&& bufferViews) noexcept;

    [[nodiscard]] static std::expected<void, Error> pushAllBuffersHelper(
        DeviceBox& deviceBox, std::vector<BufferViewMMap>& bufferViews) noexcept;

public:
    [[nodiscard]] TCAP_API static std::expected<QueueMMapBox, Error> create(std::shared_ptr<DeviceBox> pDeviceBox,
                                                                            int bufferCount = 3) noexcept;

    [[nodiscard]] TCAP_API std::expected<void, Error> startStream() noexcept;
    [[nodiscard]] TCAP_API std::expected<void, Error> stopStream() noexcept;
    [[nodiscard]] TCAP_API std::expected<void, Error> pushBuffer(BufferViewMMap& bufferView) noexcept;
    [[nodiscard]] TCAP_API std::expected<std::reference_wrapper<BufferViewMMap>, Error> popBuffer() noexcept;

private:
    std::shared_ptr<DeviceBox> pDeviceBox_;
    std::vector<BufferViewMMap> bufferViews_;
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/queue/mmap.cpp"
#endif
