#pragma once

#include <expected>
#include <memory>
#include <vector>

#include "tcap/camera/mf/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class DeviceSet {
    DeviceSet(std::vector<std::shared_ptr<DeviceBox>>&& devices) noexcept;

public:
    [[nodiscard]] TCAP_API static std::expected<DeviceSet, Error> create() noexcept;

    [[nodiscard]] TCAP_API int getDeviceCount() const noexcept { return (int)pDevices_.size(); }
    [[nodiscard]] TCAP_API std::shared_ptr<DeviceBox> getDevice(const int index) const noexcept {
        return pDevices_[index];
    }
    [[nodiscard]] TCAP_API std::shared_ptr<DeviceBox>& unsafeGetDevice(const int index) noexcept {
        return pDevices_[index];
    }
    [[nodiscard]] TCAP_API const std::shared_ptr<DeviceBox>& unsafeGetDevice(const int index) const noexcept {
        return pDevices_[index];
    }
    [[nodiscard]] TCAP_API const std::vector<std::shared_ptr<DeviceBox>>& getPDevices() const noexcept {
        return pDevices_;
    }
    [[nodiscard]] TCAP_API std::vector<std::shared_ptr<DeviceBox>>& getPDevices() noexcept { return pDevices_; }

private:
    std::vector<std::shared_ptr<DeviceBox>> pDevices_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/device/set.cpp"
#endif
