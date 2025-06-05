#include <memory>

#include <combaseapi.h>
#include <mfidl.h>
#include <mfobjects.h>

#include "tcap/camera/mf/device.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/source.hpp"
#endif

namespace tcap::mf {

SourceBox::SourceBox(std::shared_ptr<DeviceBox>&& pDeviceBox, IMFMediaSource* pSource) noexcept
    : pDeviceBox_(std::move(pDeviceBox)), pSource_(pSource) {}

SourceBox::SourceBox(SourceBox&& rhs) noexcept
    : pDeviceBox_(std::move(rhs.pDeviceBox_)), pSource_(std::exchange(rhs.pSource_, nullptr)) {}

SourceBox::~SourceBox() noexcept {
    if (pSource_ == nullptr) return;
    pSource_->Shutdown();
    pSource_->Release();
    pSource_ = nullptr;
}

std::expected<SourceBox, Error> SourceBox::create(std::shared_ptr<DeviceBox> pDeviceBox) noexcept {
    IMFMediaSource* pSource;

    auto pDevice = pDeviceBox->getPDevice();
    HRESULT hr = pDevice->ActivateObject(IID_PPV_ARGS(&pSource));
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pDevice->ActivateObject failed"}};
    }

    return SourceBox{std::move(pDeviceBox), pSource};
}

}  // namespace tcap::mf
