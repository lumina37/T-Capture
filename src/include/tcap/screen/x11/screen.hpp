#pragma once

#include <expected>
#include <vector>

#include <X11/Xlib.h>

#include "tcap/common/defines.h"
#include "tcap/screen/x11/display.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::x11 {

class ScreenBox {
public:
    TCAP_API ScreenBox(Screen* screen) noexcept;

    [[nodiscard]] TCAP_API static std::expected<std::vector<ScreenBox>, Error> createBoxes(
        const DisplayBox& displayBox) noexcept;

    [[nodiscard]] TCAP_API Screen* getScreen() const noexcept { return screen_; }
    [[nodiscard]] TCAP_API int getWidth() const noexcept { return WidthOfScreen(screen_); }
    [[nodiscard]] TCAP_API int getHeight() const noexcept { return HeightOfScreen(screen_); }
    [[nodiscard]] TCAP_API int getPlanes() const noexcept { return PlanesOfScreen(screen_); }

private:
    Screen* screen_;
};

}  // namespace tcap::x11

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/x11/screen.cpp"
#endif
