#include <expected>
#include <format>

#include <mfidl.h>
#include <mfobjects.h>

#include "tcap/helper/charset.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/device.hpp"
#endif

namespace tcap::mf {

Device::Device(IMFActivate* pDevice, std::string&& name) noexcept : pDevice_(pDevice), name_(std::move(name)) {}

Device::Device(Device&& rhs) noexcept : pDevice_(std::exchange(rhs.pDevice_, nullptr)), name_(std::move(rhs.name_)) {}

Device::~Device() noexcept {
    if (pDevice_ == nullptr) return;
    pDevice_->Release();
    pDevice_ = nullptr;
}

std::expected<Device, Error> Device::create(IMFActivate* pDevice) noexcept {
    WCHAR* pName;
    UINT32 nameLen;
    HRESULT hr = pDevice->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &pName, &nameLen);
    if (FAILED(hr)) {
        auto errMsg = std::format("pDevice_->GetAllocatedString failed");
        return std::unexpected{Error{hr, std::move(errMsg)}};
    }

    std::wstring_view utf16Name{pName, pName + nameLen};
    auto nameRes = wstringToUtf8(utf16Name);
    if (!nameRes) return std::unexpected{Error{std::move(nameRes.error())}};
    auto& name = nameRes.value();

    return Device{pDevice, std::move(name)};
}

}  // namespace tcap::mf
