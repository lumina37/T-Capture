#include <expected>

#include <mfidl.h>
#include <mfobjects.h>

#include "tcap/helper/charset.hpp"
#include "tcap/utils/error.hpp"
#include "tcap/utils/wstring.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/device/box.hpp"
#endif

namespace tcap::mf {

DeviceBox::DeviceBox(IMFActivate* pDevice, WStringBox&& uuidBox, std::string&& name) noexcept
    : pDevice_(pDevice), uuidBox_(std::move(uuidBox)), name_(std::move(name)) {}

std::expected<WStringBox, Error> DeviceBox::query(IMFActivate* pDevice, const GUID& key) noexcept {
    WCHAR* pWString;
    UINT32 len;
    const HRESULT hr = pDevice->GetAllocatedString(key, &pWString, &len);
    if (FAILED(hr)) {
        return std::unexpected{Error{ECate::eMF, hr}};
    }

    return WStringBox{pWString, len};
}

DeviceBox::DeviceBox(DeviceBox&& rhs) noexcept
    : pDevice_(std::exchange(rhs.pDevice_, nullptr)), uuidBox_(std::move(rhs.uuidBox_)), name_(std::move(rhs.name_)) {}

DeviceBox& DeviceBox::operator=(DeviceBox&& rhs) noexcept {
    pDevice_ = std::exchange(rhs.pDevice_, nullptr);
    uuidBox_ = std::move(rhs.uuidBox_);
    name_ = std::move(rhs.name_);
    return *this;
}

DeviceBox::~DeviceBox() noexcept {
    if (pDevice_ == nullptr) return;
    pDevice_->Release();
    pDevice_ = nullptr;
}

std::expected<DeviceBox, Error> DeviceBox::create(IMFActivate* pDevice) noexcept {
    pDevice->AddRef();

    auto uuidWStrBoxRes = query(pDevice, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK);
    auto& uuidWStrBox = uuidWStrBoxRes.value();

    auto nameWStrBoxRes = query(pDevice, MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME);
    auto& nameWStrBox = nameWStrBoxRes.value();

    auto nameRes = _i::wstringToUtf8(nameWStrBox.getWStringView());
    if (!nameRes) {
        pDevice->Release();
        return std::unexpected{std::move(nameRes.error())};
    }
    auto& name = nameRes.value();

    return DeviceBox{pDevice, std::move(uuidWStrBox), std::move(name)};
}

}  // namespace tcap::mf
