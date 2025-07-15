#pragma once

#include <expected>

#include "tcap/common/defines.h"
#include "tcap/screen/x11/display/box.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::x11 {

class DisplayCaps {
    DisplayCaps(bool supportImageShm) noexcept;

public:
    [[nodiscard]] TCAP_API static std::expected<DisplayCaps, Error> create(const DisplayBox& displayBox) noexcept;

    [[nodiscard]] TCAP_API bool supportImageShm() const noexcept { return supportImageShm_; }

private:
    bool supportImageShm_;
};

}  // namespace tcap::x11
