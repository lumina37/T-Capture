#pragma once

#include <expected>
#include <vector>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::v4l2 {

class NativeFormatBox {
public:
    TCAP_API NativeFormatBox(uint32_t format) noexcept;

    [[nodiscard]] TCAP_API static std::expected<std::vector<NativeFormatBox>, Error> createBoxes(
        const DeviceBox& deviceBox) noexcept;

    [[nodiscard]] TCAP_API uint32_t getFormat() const noexcept { return format_; }

private:
    uint32_t format_;
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format/native.cpp"
#endif
