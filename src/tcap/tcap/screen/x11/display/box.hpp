#pragma once

#include <expected>

#include <X11/Xlib.h>

#include "tcap/common/defines.h"
#include "tcap/screen/x11/display/box.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::x11 {

class DisplayBox {
    DisplayBox(Display* pDisplay) noexcept;

public:
    DisplayBox& operator=(const DisplayBox&) = delete;
    DisplayBox(const DisplayBox&) = delete;
    TCAP_API DisplayBox(DisplayBox&& rhs) noexcept;
    TCAP_API DisplayBox& operator=(DisplayBox&& rhs) noexcept;
    TCAP_API ~DisplayBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<DisplayBox, Error> create() noexcept;

    [[nodiscard]] TCAP_API Display* getPDisplay() const noexcept { return pDisplay_; }

private:
    Display* pDisplay_;
};

}  // namespace tcap::x11
