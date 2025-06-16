#pragma once

#include <expected>
#include <vector>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/camera/v4l2/format/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::v4l2 {

class FormatBoxes {
    FormatBoxes(std::vector<FormatBox>&& formatBoxes) noexcept;

public:
    FormatBoxes(const FormatBoxes&) = delete;
    TCAP_API FormatBoxes& operator=(FormatBoxes&& rhs) noexcept;
    TCAP_API FormatBoxes(FormatBoxes&& rhs) noexcept;

    [[nodiscard]] TCAP_API static std::expected<FormatBoxes, Error> create(const DeviceBox& deviceBox) noexcept;

    [[nodiscard]] TCAP_API int size() const noexcept { return (int)formatBoxes_.size(); }
    [[nodiscard]] TCAP_API bool empty() const noexcept { return formatBoxes_.empty(); }
    [[nodiscard]] TCAP_API const FormatBox& getFormatBox(const int index) const noexcept { return formatBoxes_[index]; }
    [[nodiscard]] TCAP_API const std::vector<FormatBox>& getFormatBoxes() const noexcept { return formatBoxes_; }
    [[nodiscard]] TCAP_API std::vector<FormatBox>& getFormatBoxes() noexcept { return formatBoxes_; }

private:
    std::vector<FormatBox> formatBoxes_;
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/format/boxes.cpp"
#endif
