#pragma once

#include <expected>
#include <memory>

#include <mfidl.h>

#include "tcap/camera/mf/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class SourceBox {
    SourceBox(std::shared_ptr<DeviceBox>&& pDeviceBox, IMFMediaSource* pSource) noexcept;

public:
    TCAP_API SourceBox(SourceBox&& rhs) noexcept;
    TCAP_API ~SourceBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<SourceBox, Error> create(
        std::shared_ptr<DeviceBox> pDeviceBox) noexcept;

    [[nodiscard]] TCAP_API IMFMediaSource* getPSource() const noexcept { return pSource_; }

private:
    std::shared_ptr<DeviceBox> pDeviceBox_;
    IMFMediaSource* pSource_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/source.cpp"
#endif
