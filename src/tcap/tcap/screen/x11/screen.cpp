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

ScreenBox::ScreenBox(Screen* pScreen) noexcept : pScreen_(pScreen) {}

std::expected<std::vector<ScreenBox>, Error> ScreenBox::createBoxes(const DisplayBox& displayBox) noexcept {
    Display* pDisplay = displayBox.getPDisplay();

    const int screenCount = XScreenCount(pDisplay);

    std::vector<ScreenBox> screenBoxes;
    screenBoxes.reserve(screenCount);
    for (int screenIndex = 0; screenIndex < screenCount; screenIndex++) {
        Screen* pScreen = ScreenOfDisplay(pDisplay, screenIndex);
        screenBoxes.emplace_back(pScreen);
    }

    return screenBoxes;
}

}  // namespace tcap::x11
