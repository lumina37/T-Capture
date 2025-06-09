#include <memory>

#include <atlbase.h>
#include <mfidl.h>
#include <mfobjects.h>

#include "tcap/camera/mf/device/box.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/source.hpp"
#endif

namespace tcap::mf {

SourceBox::SourceBox(std::shared_ptr<DeviceBox>&& pDeviceBox, CComPtr<IMFMediaSource>&& pSource) noexcept
    : pDeviceBox_(std::move(pDeviceBox)), pSource_(std::move(pSource)) {}

SourceBox::~SourceBox() noexcept {
    if (pSource_ == nullptr) return;
    pSource_->Shutdown();
    pSource_ = nullptr;
}

std::expected<SourceBox, Error> SourceBox::create(std::shared_ptr<DeviceBox> pDeviceBox) noexcept {
    auto pDevice = pDeviceBox->getPDevice();
    CComPtr<IMFMediaSource> pSource;
    HRESULT hr = pDevice->ActivateObject(IID_PPV_ARGS(&pSource));
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pDevice->ActivateObject failed"}};
    }

    return SourceBox{std::move(pDeviceBox), std::move(pSource)};
}

}  // namespace tcap::mf
