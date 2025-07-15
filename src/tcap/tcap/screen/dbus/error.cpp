#include <utility>

#include <dbus/dbus.h>

#include "tcap/screen/dbus/error.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::dbus {

ErrorBox::ErrorBox() noexcept { dbus_error_init(&error_); }

ErrorBox::ErrorBox(ErrorBox&& rhs) noexcept { dbus_move_error(&rhs.error_, &error_); }

ErrorBox& ErrorBox::operator=(ErrorBox&& rhs) noexcept {
    dbus_move_error(&rhs.error_, &error_);
    return *this;
}

ErrorBox::~ErrorBox() noexcept {
    if (!dbus_error_is_set(&error_)) return;
    dbus_error_free(&error_);
}

}  // namespace tcap::dbus
