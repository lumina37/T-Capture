#pragma once

#include <expected>
#include <string>

#include <mfobjects.h>

#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class DeviceBox {
    DeviceBox(IMFActivate* pDevice, std::string&& name) noexcept;

public:
    TCAP_API DeviceBox(DeviceBox&& rhs) noexcept;
    TCAP_API ~DeviceBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<DeviceBox, Error> create(IMFActivate* pDevice) noexcept;

    [[nodiscard]] TCAP_API std::string_view getName() const noexcept { return name_; }

private:
    IMFActivate* pDevice_;

    std::string name_;  // utf-8
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/device/box.cpp"
#endif
