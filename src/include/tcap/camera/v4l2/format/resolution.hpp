#pragma once

#include <expected>
#include <vector>

#include <linux/videodev2.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/camera/v4l2/format/fps.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::v4l2 {

class ResolutionBox {
    ResolutionBox(uint32_t width, uint32_t height, std::vector<Fps>&& fpss) noexcept;

public:
    ResolutionBox(const ResolutionBox&) = delete;
    TCAP_API ResolutionBox& operator=(ResolutionBox&& rhs) noexcept;
    TCAP_API ResolutionBox(ResolutionBox&& rhs) noexcept;

    [[nodiscard]] TCAP_API static std::expected<ResolutionBox, Error> create(const DeviceBox& deviceBox,
                                                                             const v4l2_fmtdesc& fmtDesc,
                                                                             const v4l2_frmsizeenum& frmSize) noexcept;

private:
    uint32_t width_;
    uint32_t height_;
    std::vector<Fps> fpss_;
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format/resolution.cpp"
#endif
