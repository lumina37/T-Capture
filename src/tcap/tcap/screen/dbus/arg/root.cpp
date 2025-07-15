#include <dbus/dbus.h>

#include "tcap/screen/dbus/arg/root.hpp"

namespace tcap::dbus::_i {

MsgArgRootSetter::MsgArgRootSetter(DBusMessage* pMessage) noexcept { dbus_message_iter_init_append(pMessage, &self_); }

MsgArgRootGetter::MsgArgRootGetter(DBusMessage* pMessage) noexcept { dbus_message_iter_init(pMessage, &self_); }

}  // namespace tcap::dbus::_i
