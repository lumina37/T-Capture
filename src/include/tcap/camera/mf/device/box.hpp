#pragma once

#include <expected>
#include <string>

#include <atlbase.h>
#include <mfobjects.h>

#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"
#include "tcap/helper/mf/wstring.hpp"

namespace tcap::mf {

class DeviceBox {
    DeviceBox(CComPtr<IMFActivate>&& pDevice, WStringBox&& uuidBox, std::string&& name) noexcept;

    [[nodiscard]] static std::expected<WStringBox, Error> query(IMFActivate* pDevice, const GUID& key) noexcept;

public:
    DeviceBox(const DeviceBox&) = delete;
    TCAP_API DeviceBox(DeviceBox&&) noexcept = default;

    [[nodiscard]] TCAP_API static std::expected<DeviceBox, Error> create(CComPtr<IMFActivate>&& pDevice) noexcept;

    [[nodiscard]] TCAP_API IMFActivate* getPDevice() const noexcept { return pDevice_; }
    [[nodiscard]] TCAP_API WStringBox& getUuidBox() noexcept { return uuidBox_; }
    [[nodiscard]] TCAP_API const WStringBox& getUuidBox() const noexcept { return uuidBox_; }
    [[nodiscard]] TCAP_API std::string_view getName() const noexcept { return name_; }

private:
    CComPtr<IMFActivate> pDevice_;

    WStringBox uuidBox_;
    std::string name_;  // utf-8
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/device/box.cpp"
#endif
