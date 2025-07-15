#include <dbus/dbus.h>

#include "tcap/screen/dbus/arg/array.hpp"
#include "tcap/screen/dbus/arg/root.hpp"

namespace tcap::dbus::_i {

MsgArgArray::MsgArgArray(MsgArgRootSetter& parent) noexcept : parent_(parent) {
    dbus_message_iter_open_container(parent.getPSelf(), DBUS_TYPE_ARRAY, "{sv}", &self_);
}

MsgArgArray::~MsgArgArray() noexcept { dbus_message_iter_close_container(parent_.getPSelf(), &self_); }

}  // namespace tcap::dbus::_i
