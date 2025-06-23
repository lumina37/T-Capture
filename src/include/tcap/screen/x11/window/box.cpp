#include <expected>

#include <X11/Xlib.h>

#include "tcap/screen/x11/display.hpp"
#include "tcap/screen/x11/screen.hpp"
#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/x11/window/box.hpp"
#endif

namespace tcap::x11 {

WindowBox::WindowBox(const Window window, const int width, const int height, const int planes) noexcept
    : window_(window), width_(width), height_(height), planes_(planes) {}

std::expected<WindowBox, Error> WindowBox::create(const DisplayBox &displayBox, Window window) noexcept {
    Display *display = displayBox.getDisplay();
    XWindowAttributes attr;
    XGetWindowAttributes(display, window, &attr);

    return WindowBox{window, attr.width, attr.height, attr.depth};
}

std::expected<WindowBox, Error> WindowBox::createRoot(const DisplayBox &displayBox,
                                                      const ScreenBox &screenBox) noexcept {
    Screen *screen = screenBox.getScreen();
    Window rootWindow = RootWindowOfScreen(screen);
    return create(displayBox, rootWindow);
}

}  // namespace tcap::x11
