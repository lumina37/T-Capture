#include <format>
#include <memory>
#include <string>

#include <mfidl.h>
#include <mfobjects.h>

#include "tcap/camera/mf/device/box.hpp"
#include "tcap/helper/mf/attributes.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/device/set.hpp"
#endif

namespace tcap::mf {

DeviceSet::DeviceSet(std::vector<std::shared_ptr<DeviceBox>>&& devices) noexcept : pDevices_(std::move(devices)) {}

std::expected<DeviceSet, Error> DeviceSet::create() noexcept {
    IMFActivate** pDevices;

    auto attrsBoxRes = AttributesBox::create(1);
    if (!attrsBoxRes) return std::unexpected{std::move(attrsBoxRes.error())};
    auto& attrsBox = attrsBoxRes.value();

    auto setRes = attrsBox.set(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
    if (!setRes) return std::unexpected{std::move(setRes.error())};

    UINT32 deviceCount = 0;
    HRESULT hr = MFEnumDeviceSources(attrsBox.getPAttributes(), &pDevices, &deviceCount);
    if (FAILED(hr)) {
        auto errMsg = std::format("MFEnumDeviceSources failed");
        return std::unexpected{Error{hr, std::move(errMsg)}};
    }

    std::vector<std::shared_ptr<DeviceBox>> devices;
    devices.reserve(deviceCount);
    for (UINT32 i = 0; i < deviceCount; i++) {
        auto deviceRes = DeviceBox::create(pDevices[i]);
        if (!deviceRes) return std::unexpected{std::move(deviceRes.error())};
        auto pDevice = std::make_shared<DeviceBox>(std::move(deviceRes.value()));
        devices.push_back(std::move(pDevice));
    }

    CoTaskMemFree(pDevices);

    return DeviceSet{std::move(devices)};
}

}  // namespace tcap::mf
