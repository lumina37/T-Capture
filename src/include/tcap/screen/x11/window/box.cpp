#include <expected>

#include <X11/Xlib.h>

#include "tcap/screen/x11/display/box.hpp"
#include "tcap/screen/x11/screen.hpp"
#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/x11/window/box.hpp"
#endif

namespace tcap::x11 {

WindowBox::WindowBox(const Window window, const XWindowAttributes &attrs) noexcept : window_(window), attrs_(attrs) {}

std::expected<WindowBox, Error> WindowBox::create(const DisplayBox &displayBox, Window window) noexcept {
    Display *display = displayBox.getPDisplay();
    XWindowAttributes attrs;
    XGetWindowAttributes(display, window, &attrs);

    return WindowBox{window, attrs};
}

std::expected<WindowBox, Error> WindowBox::createRoot(const DisplayBox &displayBox,
                                                      const ScreenBox &screenBox) noexcept {
    Screen *pScreen = screenBox.getPScreen();
    Window rootWindow = RootWindowOfScreen(pScreen);
    return create(displayBox, rootWindow);
}

}  // namespace tcap::x11
