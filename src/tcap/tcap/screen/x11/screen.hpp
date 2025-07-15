#pragma once

#include <expected>
#include <vector>

#include <X11/Xlib.h>

#include "tcap/common/defines.h"
#include "tcap/screen/x11/display/box.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::x11 {

class ScreenBox {
public:
    TCAP_API ScreenBox(Screen* pScreen) noexcept;

    [[nodiscard]] TCAP_API static std::expected<std::vector<ScreenBox>, Error> createBoxes(
        const DisplayBox& displayBox) noexcept;

    [[nodiscard]] TCAP_API Screen* getPScreen() const noexcept { return pScreen_; }
    [[nodiscard]] TCAP_API int getWidth() const noexcept { return WidthOfScreen(pScreen_); }
    [[nodiscard]] TCAP_API int getHeight() const noexcept { return HeightOfScreen(pScreen_); }
    [[nodiscard]] TCAP_API int getPlanes() const noexcept { return PlanesOfScreen(pScreen_); }

private:
    Screen* pScreen_;
};

}  // namespace tcap::x11
