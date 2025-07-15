#include <expected>

#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>

#include "tcap/screen/x11/display/caps.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::x11 {

DisplayCaps::DisplayCaps(bool supportImageShm) noexcept : supportImageShm_(supportImageShm) {}

std::expected<DisplayCaps, Error> DisplayCaps::create(const DisplayBox& displayBox) noexcept {
    Display* pDisplay = displayBox.getPDisplay();
    bool supportImageShm = XShmQueryExtension(pDisplay);
    return DisplayCaps{supportImageShm};
}

}  // namespace tcap::x11
