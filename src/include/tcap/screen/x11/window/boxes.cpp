#include <expected>
#include <utility>
#include <vector>

#include <X11/Xlib.h>

#include "tcap/screen/x11/display.hpp"
#include "tcap/screen/x11/screen.hpp"
#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/x11/window/boxes.hpp"
#endif

namespace tcap::x11 {

TopWindowBoxes::TopWindowBoxes(std::vector<WindowBox>&& windowBoxes) noexcept : windowBoxes_(std::move(windowBoxes)) {}

std::expected<TopWindowBoxes, Error> TopWindowBoxes::create(const DisplayBox& displayBox,
                                                            const ScreenBox& screenBox) noexcept {
    Display* display = displayBox.getDisplay();
    Screen* screen = screenBox.getScreen();
    Window rootWindow = RootWindowOfScreen(screen);

    Window rootRet, parentRet;
    Window* children;
    unsigned int childrenCount;

    const Status status = XQueryTree(display, rootWindow, &rootRet, &parentRet, &children, &childrenCount);
    if (status == 0) {
        return std::unexpected{Error{ECate::eX11, 0}};
    }

    std::vector<WindowBox> windowBoxes;
    windowBoxes.reserve(childrenCount);
    for (int childrenIndex = 0; childrenIndex < (int)childrenCount; childrenIndex++) {
        Window window = *(children + childrenIndex);
        auto windowBoxRes = WindowBox::create(displayBox, window);
        if (!windowBoxRes) return std::unexpected{std::move(windowBoxRes.error())};
        windowBoxes.push_back(windowBoxRes.value());
    }

    return TopWindowBoxes{std::move(windowBoxes)};
}

}  // namespace tcap::x11
