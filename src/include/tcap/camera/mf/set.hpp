#pragma once

#include <expected>
#include <span>
#include <vector>

#include <mfobjects.h>

#include "tcap/camera/mf/device.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class DeviceSet {
    DeviceSet(IMFActivate** pDevices, std::vector<Device>&& devices) noexcept;

public:
    TCAP_API DeviceSet(DeviceSet&& rhs) noexcept;
    TCAP_API ~DeviceSet() noexcept;

    [[nodiscard]] TCAP_API static std::expected<DeviceSet, Error> create() noexcept;

    [[nodiscard]] TCAP_API std::span<const Device> getDevices() const noexcept { return devices_; }

private:
    IMFActivate** pDevices_;

    std::vector<Device> devices_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/set.cpp"
#endif
