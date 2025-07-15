#pragma once

#include <expected>

#include <X11/Xlib.h>

#include "tcap/screen/x11/display/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/screen/x11/screen.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::x11 {

class WindowBox {
    WindowBox(Window window, const XWindowAttributes& attrs) noexcept;

public:
    [[nodiscard]] TCAP_API static std::expected<WindowBox, Error> create(const DisplayBox& displayBox,
                                                                         Window window) noexcept;
    [[nodiscard]] TCAP_API static std::expected<WindowBox, Error> createRoot(const DisplayBox& displayBox,
                                                                             const ScreenBox& screenBox) noexcept;

    [[nodiscard]] TCAP_API Window getWindow() const noexcept { return window_; }
    [[nodiscard]] TCAP_API Visual* getVisual() const noexcept { return attrs_.visual; }
    [[nodiscard]] TCAP_API int getWidth() const noexcept { return attrs_.width; }
    [[nodiscard]] TCAP_API int getHeight() const noexcept { return attrs_.height; }
    [[nodiscard]] TCAP_API int getPlanes() const noexcept { return attrs_.depth; }

private:
    Window window_;
    XWindowAttributes attrs_;
};

}  // namespace tcap::x11
