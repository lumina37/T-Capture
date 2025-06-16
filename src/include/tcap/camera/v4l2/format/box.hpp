#pragma once

#include <expected>
#include <vector>

#include <linux/videodev2.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/camera/v4l2/format/resolution.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::v4l2 {

class FormatBox {
    FormatBox(uint32_t format, std::vector<ResolutionBox>&& resolutionBoxes) noexcept;

public:
    FormatBox(const FormatBox&) = delete;
    TCAP_API FormatBox& operator=(FormatBox&& rhs) noexcept;
    TCAP_API FormatBox(FormatBox&& rhs) noexcept;

    [[nodiscard]] TCAP_API static std::expected<FormatBox, Error> create(const DeviceBox& deviceBox,
                                                                         const v4l2_fmtdesc& fmtDesc) noexcept;

private:
    uint32_t format_;
    std::vector<ResolutionBox> resolutionBoxes_;
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format/box.cpp"
#endif
