#include <print>

#include <mfapi.h>
#include <mfidl.h>
#include <mfobjects.h>
#include <string>

#include "tcap/camera/mf/device/box.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/device/set.hpp"
#endif

namespace tcap::mf {

DeviceSet::DeviceSet(IMFActivate** pDevices, std::vector<DeviceBox>&& devices) noexcept
    : pDevices_(pDevices), devices_(std::move(devices)) {}

DeviceSet::DeviceSet(DeviceSet&& rhs) noexcept
    : pDevices_(std::exchange(rhs.pDevices_, nullptr)), devices_(std::move(rhs.devices_)) {}

DeviceSet::~DeviceSet() noexcept {
    if (pDevices_ == nullptr) return;
    CoTaskMemFree(pDevices_);
    pDevices_ = nullptr;
}

std::expected<DeviceSet, Error> DeviceSet::create() noexcept {
    IMFAttributes* pAttributes;
    IMFActivate** pDevices = nullptr;
    HRESULT hr;

    hr = MFCreateAttributes(&pAttributes, 1);
    if (FAILED(hr)) {
        auto errMsg = std::format("MFCreateAttributes failed");
        return std::unexpected{Error{hr, std::move(errMsg)}};
    }

    hr = pAttributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);

    if (FAILED(hr)) {
        pAttributes->Release();  // TODO: wrap IMFAttributes into RAII
        auto errMsg = std::format("pAttributes->SetGUID failed");
        return std::unexpected{Error{hr, std::move(errMsg)}};
    }

    UINT32 deviceCount = 0;
    hr = MFEnumDeviceSources(pAttributes, &pDevices, &deviceCount);
    if (FAILED(hr)) {
        pAttributes->Release();
        auto errMsg = std::format("MFEnumDeviceSources failed");
        return std::unexpected{Error{hr, std::move(errMsg)}};
    }
    pAttributes->Release();

    std::vector<DeviceBox> devices;
    devices.reserve(deviceCount);
    for (UINT32 i = 0; i < deviceCount; i++) {
        auto deviceRes = DeviceBox::create(pDevices[i]);
        if (!deviceRes) return std::unexpected{std::move(deviceRes.error())};
        devices.push_back(std::move(deviceRes.value()));
    }

    return DeviceSet{pDevices, std::move(devices)};
}

}  // namespace tcap::mf
