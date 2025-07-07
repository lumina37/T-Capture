#pragma once

#include <expected>
#include <vector>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::v4l2 {

class FormatNativeBox {
public:
    TCAP_API constexpr FormatNativeBox(uint32_t format) noexcept;

    [[nodiscard]] TCAP_API static std::expected<std::vector<FormatNativeBox>, Error> createBoxes(
        const DeviceBox& deviceBox) noexcept;

    [[nodiscard]] TCAP_API constexpr uint32_t getFormat() const noexcept { return format_; }

private:
    uint32_t format_;
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format/native.cpp"
#endif
