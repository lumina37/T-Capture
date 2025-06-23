#pragma once

#include <expected>

#include <X11/Xlib.h>

#include "tcap/common/defines.h"
#include "tcap/screen/x11/screen.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::x11 {

class WindowBoxes {
    WindowBoxes(Window rootWindow, Window* topWindows, int topWindowsCount) noexcept;

public:
    WindowBoxes& operator=(const WindowBoxes&) = delete;
    WindowBoxes(const WindowBoxes&) = delete;
    TCAP_API WindowBoxes(WindowBoxes&& rhs) noexcept;
    TCAP_API WindowBoxes& operator=(WindowBoxes&& rhs) noexcept;
    TCAP_API ~WindowBoxes() noexcept;

    [[nodiscard]] TCAP_API static std::expected<WindowBoxes, Error> create(const DisplayBox& displayBox,
                                                                           const ScreenBox& screenBox) noexcept;

    [[nodiscard]] TCAP_API Window getRootWindow() const noexcept { return rootWindow_; }
    [[nodiscard]] TCAP_API int getTopWindowsCount() const noexcept { return topWindowsCount_; }
    [[nodiscard]] TCAP_API Window getTopWindow(const int index) const noexcept { return *(topWindows_ + index); }

private:
    Window rootWindow_;
    Window* topWindows_;
    int topWindowsCount_;
};

}  // namespace tcap::x11

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/x11/window/boxes.cpp"
#endif
