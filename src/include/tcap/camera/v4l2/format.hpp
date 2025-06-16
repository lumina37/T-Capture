#pragma once

#include <expected>
#include <vector>

#include <linux/videodev2.h>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::v4l2 {

class FormatBox {
    FormatBox(std::vector<v4l2_fmtdesc>&& fmtDescs) noexcept;

public:
    FormatBox(const FormatBox&) = delete;
    TCAP_API FormatBox& operator=(FormatBox&& rhs) noexcept;
    TCAP_API FormatBox(FormatBox&& rhs) noexcept;

    [[nodiscard]] TCAP_API static std::expected<FormatBox, Error> create(const DeviceBox& deviceBox) noexcept;

    [[nodiscard]] TCAP_API int size() const noexcept { return (int)fmtDescs_.size(); }
    [[nodiscard]] TCAP_API bool empty() const noexcept { return fmtDescs_.empty(); }
    [[nodiscard]] TCAP_API const v4l2_fmtdesc& getPath(const int index) const noexcept { return fmtDescs_[index]; }
    [[nodiscard]] TCAP_API const std::vector<v4l2_fmtdesc>& getPaths() const noexcept { return fmtDescs_; }
    [[nodiscard]] TCAP_API std::vector<v4l2_fmtdesc>& getPaths() noexcept { return fmtDescs_; }

private:
    std::vector<v4l2_fmtdesc> fmtDescs_;
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format.cpp"
#endif
