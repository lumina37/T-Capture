#include <memory>

#include <mfidl.h>
#include <mfobjects.h>

#include "tcap/camera/mf/device/box.hpp"
#include "tcap/camera/mf/source.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::mf {

SourceBox::SourceBox(std::shared_ptr<DeviceBox>&& pDeviceBox, IMFMediaSource* pSource) noexcept
    : pDeviceBox_(std::move(pDeviceBox)), pSource_(pSource) {}

SourceBox& SourceBox::operator=(SourceBox&& rhs) noexcept {
    pDeviceBox_ = std::move(rhs.pDeviceBox_);
    pSource_ = std::exchange(rhs.pSource_, nullptr);
    return *this;
}

SourceBox::SourceBox(SourceBox&& rhs) noexcept
    : pDeviceBox_(std::move(rhs.pDeviceBox_)), pSource_(std::exchange(rhs.pSource_, nullptr)) {}

SourceBox::~SourceBox() noexcept {
    if (pSource_ == nullptr) return;
    pSource_->Shutdown();
    pSource_->Release();
    pSource_ = nullptr;
}

std::expected<SourceBox, Error> SourceBox::create(std::shared_ptr<DeviceBox> pDeviceBox) noexcept {
    auto pDevice = pDeviceBox->getPDevice();
    IMFMediaSource* pSource;
    HRESULT hr = pDevice->ActivateObject(IID_PPV_ARGS(&pSource));
    if (FAILED(hr)) {
        return std::unexpected{Error{ECate::eMF, hr}};
    }
    pSource->AddRef();

    return SourceBox{std::move(pDeviceBox), pSource};
}

}  // namespace tcap::mf
