#pragma once

#include <expected>
#include <memory>
#include <vector>

#include "tcap/camera/mf/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::mf {

class DeviceBoxes {
    DeviceBoxes(std::vector<std::shared_ptr<DeviceBox>>&& deviceBoxes) noexcept;

public:
    DeviceBoxes(const DeviceBoxes&) = delete;
    DeviceBoxes& operator=(const DeviceBoxes&) = delete;
    TCAP_API DeviceBoxes(DeviceBoxes&&) noexcept = default;
    TCAP_API DeviceBoxes& operator=(DeviceBoxes&&) noexcept = default;

    [[nodiscard]] TCAP_API static std::expected<DeviceBoxes, Error> create() noexcept;

    [[nodiscard]] TCAP_API int size() const noexcept { return (int)pDeviceBoxes_.size(); }
    [[nodiscard]] TCAP_API bool empty() const noexcept { return pDeviceBoxes_.empty(); }
    [[nodiscard]] TCAP_API std::shared_ptr<DeviceBox> getPDeviceBox(const int index) const noexcept {
        return pDeviceBoxes_[index];
    }
    [[nodiscard]] TCAP_API std::shared_ptr<DeviceBox>& unsafeGetPDeviceBox(const int index) noexcept {
        return pDeviceBoxes_[index];
    }
    [[nodiscard]] TCAP_API const std::shared_ptr<DeviceBox>& unsafeGetPDeviceBox(const int index) const noexcept {
        return pDeviceBoxes_[index];
    }
    [[nodiscard]] TCAP_API const std::vector<std::shared_ptr<DeviceBox>>& getPDeviceBoxes() const noexcept {
        return pDeviceBoxes_;
    }
    [[nodiscard]] TCAP_API std::vector<std::shared_ptr<DeviceBox>>& getPDeviceBoxes() noexcept { return pDeviceBoxes_; }

private:
    std::vector<std::shared_ptr<DeviceBox>> pDeviceBoxes_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/device/boxes.cpp"
#endif
