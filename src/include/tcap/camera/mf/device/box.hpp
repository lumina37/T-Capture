#pragma once

#include <expected>
#include <string>

#include <mfobjects.h>

#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"
#include "tcap/helper/mf/wstring.hpp"

namespace tcap::mf {

class DeviceBox {
    DeviceBox(IMFActivate* pDevice, WStringBox&& uuidBox, std::string&& name) noexcept;

    [[nodiscard]] static std::expected<WStringBox, Error> query(IMFActivate* pDevice, const GUID& key) noexcept;

public:
    DeviceBox& operator=(const DeviceBox&) = delete;
    DeviceBox(const DeviceBox&) = delete;
    TCAP_API DeviceBox(DeviceBox&& rhs) noexcept;
    TCAP_API DeviceBox& operator=(DeviceBox&& rhs) noexcept;
    TCAP_API ~DeviceBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<DeviceBox, Error> create(IMFActivate* pDevice) noexcept;

    [[nodiscard]] TCAP_API IMFActivate* getPDevice() const noexcept { return pDevice_; }
    [[nodiscard]] TCAP_API WStringBox& getUuidBox() noexcept { return uuidBox_; }
    [[nodiscard]] TCAP_API const WStringBox& getUuidBox() const noexcept { return uuidBox_; }
    [[nodiscard]] TCAP_API std::string_view getName() const noexcept { return name_; }

private:
    IMFActivate* pDevice_;

    WStringBox uuidBox_;
    std::string name_;  // utf-8
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/device/box.cpp"
#endif
