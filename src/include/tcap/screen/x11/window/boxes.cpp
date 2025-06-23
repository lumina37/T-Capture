#include <expected>
#include <utility>

#include <X11/Xlib.h>

#include "tcap/screen/x11/screen.hpp"
#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/x11/window/boxes.hpp"
#endif

namespace tcap::x11 {

WindowBoxes::WindowBoxes(Window rootWindow, Window* topWindows, int topWindowsCount) noexcept
    : rootWindow_(rootWindow), topWindows_(topWindows), topWindowsCount_(topWindowsCount) {}

WindowBoxes::WindowBoxes(WindowBoxes&& rhs) noexcept
    : rootWindow_(rhs.rootWindow_),
      topWindows_(std::exchange(rhs.topWindows_, nullptr)),
      topWindowsCount_(rhs.topWindowsCount_) {}

WindowBoxes& WindowBoxes::operator=(WindowBoxes&& rhs) noexcept {
    rootWindow_ = rhs.rootWindow_;
    topWindows_ = std::exchange(rhs.topWindows_, nullptr);
    topWindowsCount_ = rhs.topWindowsCount_;
    return *this;
}

WindowBoxes::~WindowBoxes() noexcept {
    if (topWindows_ == nullptr) return;
    XFree(topWindows_);
    topWindows_ = nullptr;
}

std::expected<WindowBoxes, Error> WindowBoxes::create(const DisplayBox& displayBox,
                                                      const ScreenBox& screenBox) noexcept {
    Display* display = displayBox.getDisplay();
    Screen* screen = screenBox.getScreen();

    Window rootWindow = RootWindowOfScreen(screen);

    Window rootRet, parentRet;
    Window* children;
    unsigned int childrenCount;

    Status status = XQueryTree(display, rootWindow, &rootRet, &parentRet, &children, &childrenCount);
    if (status == 0) {
        return std::unexpected{Error{ECate::eX11, 0}};
    }

    return WindowBoxes{rootWindow, children, (int)childrenCount};
}

}  // namespace tcap::x11
