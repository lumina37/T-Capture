#include <expected>

#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>

#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/x11/display/caps.hpp"
#endif

namespace tcap::x11 {

DisplayCaps::DisplayCaps(bool supportImageShm) noexcept : supportImageShm_(supportImageShm) {}

std::expected<DisplayCaps, Error> DisplayCaps::create(const DisplayBox& displayBox) noexcept {
    Display* display = displayBox.getDisplay();
    bool supportImageShm = XShmQueryExtension(display);
    return DisplayCaps{supportImageShm};
}

}  // namespace tcap::x11
