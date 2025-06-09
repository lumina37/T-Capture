#include <expected>

#include <atlbase.h>
#include <mfidl.h>
#include <mfobjects.h>

#include "tcap/helper/charset.hpp"
#include "tcap/helper/error.hpp"
#include "tcap/helper/mf/wstring.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/device/box.hpp"
#endif

namespace tcap::mf {

DeviceBox::DeviceBox(CComPtr<IMFActivate>&& pDevice, WStringBox&& uuidBox, std::string&& name) noexcept
    : pDevice_(std::move(pDevice)), uuidBox_(std::move(uuidBox)), name_(std::move(name)) {}

std::expected<WStringBox, Error> DeviceBox::query(IMFActivate* pDevice, const GUID& key) noexcept {
    WCHAR* pWString;
    UINT32 len;
    const HRESULT hr = pDevice->GetAllocatedString(key, &pWString, &len);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pDevice_->GetAllocatedString failed"}};
    }

    return WStringBox{pWString, len};
}

std::expected<DeviceBox, Error> DeviceBox::create(CComPtr<IMFActivate>&& pDevice) noexcept {
    auto uuidWStrBoxRes = query(pDevice, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK);
    if (!uuidWStrBoxRes) return std::unexpected{Error{std::move(uuidWStrBoxRes.error())}};
    auto& uuidWStrBox = uuidWStrBoxRes.value();

    auto nameWStrBoxRes = query(pDevice, MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME);
    if (!nameWStrBoxRes) return std::unexpected{Error{std::move(nameWStrBoxRes.error())}};
    auto& nameWStrBox = nameWStrBoxRes.value();

    auto nameRes = wstringToUtf8(nameWStrBox.getWStringView());
    if (!nameRes) return std::unexpected{Error{std::move(nameRes.error())}};
    auto& name = nameRes.value();

    return DeviceBox{std::move(pDevice), std::move(uuidWStrBox), std::move(name)};
}

}  // namespace tcap::mf
