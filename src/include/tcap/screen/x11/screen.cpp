#include <expected>
#include <utility>
#include <vector>

#include <X11/Xlib.h>

#include "tcap/screen/x11/display/box.hpp"
#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/x11/screen.hpp"
#endif

namespace tcap::x11 {

ScreenBox::ScreenBox(Screen* screen) noexcept : screen_(screen) {}

std::expected<std::vector<ScreenBox>, Error> ScreenBox::createBoxes(const DisplayBox& displayBox) noexcept {
    Display* display = displayBox.getDisplay();

    const int screenCount = XScreenCount(display);

    std::vector<ScreenBox> screenBoxes;
    screenBoxes.reserve(screenCount);
    for (int screenIndex = 0; screenIndex < screenCount; screenIndex++) {
        Screen* screen = ScreenOfDisplay(display, screenIndex);
        screenBoxes.emplace_back(screen);
    }

    return screenBoxes;
}

}  // namespace tcap::x11
