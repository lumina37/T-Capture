#include <memory>

#include <mfidl.h>
#include <mfobjects.h>

#include "tcap/helper/error.hpp"
#include "tcap/helper/mf/attributes.hpp"
#include "tcap/platforms/mf/device/box.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/device/boxes.hpp"
#endif

namespace tcap::mf {

DeviceBoxes::DeviceBoxes(std::vector<std::shared_ptr<DeviceBox>>&& deviceBoxes) noexcept
    : pDeviceBoxes_(std::move(deviceBoxes)) {}

std::expected<DeviceBoxes, Error> DeviceBoxes::create() noexcept {
    IMFActivate** pDevices;

    auto attrsBoxRes = AttributesBox::create(1);
    if (!attrsBoxRes) return std::unexpected{std::move(attrsBoxRes.error())};
    auto& attrsBox = attrsBoxRes.value();

    auto setRes = attrsBox.set(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
    if (!setRes) return std::unexpected{std::move(setRes.error())};

    UINT32 deviceCount = 0;
    HRESULT hr = MFEnumDeviceSources(attrsBox.getPAttributes(), &pDevices, &deviceCount);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "MFEnumDeviceSources failed"}};
    }

    std::vector<std::shared_ptr<DeviceBox>> deviceBoxes;
    deviceBoxes.reserve(deviceCount);
    for (UINT32 i = 0; i < deviceCount; i++) {
        auto deviceBoxRes = DeviceBox::create(pDevices[i]);
        if (!deviceBoxRes) return std::unexpected{std::move(deviceBoxRes.error())};
        auto pDeviceBox = std::make_shared<DeviceBox>(std::move(deviceBoxRes.value()));
        deviceBoxes.push_back(std::move(pDeviceBox));
    }

    CoTaskMemFree(pDevices);

    return DeviceBoxes{std::move(deviceBoxes)};
}

}  // namespace tcap::mf
