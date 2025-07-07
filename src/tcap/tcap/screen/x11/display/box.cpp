#include <cstdlib>
#include <expected>
#include <format>
#include <utility>

#include <X11/Xlib.h>

#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/x11/display/box.hpp"
#endif

namespace tcap::x11 {

DisplayBox::DisplayBox(Display* pDisplay) noexcept : pDisplay_(pDisplay) {}

DisplayBox::DisplayBox(DisplayBox&& rhs) noexcept : pDisplay_(std::exchange(rhs.pDisplay_, nullptr)) {}

DisplayBox& DisplayBox::operator=(DisplayBox&& rhs) noexcept {
    pDisplay_ = std::exchange(rhs.pDisplay_, nullptr);
    return *this;
}

DisplayBox::~DisplayBox() noexcept {
    if (pDisplay_ == nullptr) return;
    XCloseDisplay(pDisplay_);
    pDisplay_ = nullptr;
}

std::expected<DisplayBox, Error> DisplayBox::create() noexcept {
    Display* pDisplay = XOpenDisplay(nullptr);
    if (pDisplay == nullptr) {
        const char* displayName = std::getenv("DISPLAY");
        if (displayName == nullptr) {
            return std::unexpected{Error{ECate::eX11, 0, "env DISPLAY is null"}};
        }
        auto errMsg = std::format("cannot open DISPLAY={}", displayName);
        return std::unexpected{Error{ECate::eX11, 0, std::move(errMsg)}};
    }

    return DisplayBox{pDisplay};
}

}  // namespace tcap::x11
