#pragma once

#include <expected>
#include <string>

#include <mfobjects.h>

#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class Device {
    Device(IMFActivate* pDevice, std::string&& name) noexcept;

public:
    TCAP_API Device(Device&& rhs) noexcept;
    TCAP_API ~Device() noexcept;

    [[nodiscard]] TCAP_API static std::expected<Device, Error> create(IMFActivate* pDevice) noexcept;

    [[nodiscard]] TCAP_API std::string_view getName() const noexcept { return name_; }

private:
    IMFActivate* pDevice_;

    std::string name_;
};

}  // namespace tcap::mf
