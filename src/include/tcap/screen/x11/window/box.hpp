#pragma once

#include <expected>

#include <X11/Xlib.h>

#include "tcap/common/defines.h"
#include "tcap/screen/x11/display.hpp"
#include "tcap/screen/x11/screen.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::x11 {

class WindowBox {
    WindowBox(Window window, int width, int height, int planes) noexcept;

public:
    [[nodiscard]] TCAP_API static std::expected<WindowBox, Error> create(const DisplayBox& displayBox,
                                                                         Window window) noexcept;
    [[nodiscard]] TCAP_API static std::expected<WindowBox, Error> createRoot(const DisplayBox& displayBox,
                                                                             const ScreenBox& screenBox) noexcept;

    [[nodiscard]] TCAP_API Window getWindow() const noexcept { return window_; }
    [[nodiscard]] TCAP_API int getWidth() const noexcept { return width_; }
    [[nodiscard]] TCAP_API int getHeight() const noexcept { return height_; }
    [[nodiscard]] TCAP_API int getPlanes() const noexcept { return planes_; }

private:
    Window window_;
    int width_;
    int height_;
    int planes_;
};

}  // namespace tcap::x11

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/x11/window/box.cpp"
#endif
