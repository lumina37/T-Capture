#include <dbus/dbus.h>

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/dbus/arg/root.hpp"
#endif

namespace tcap::dbus::_i {

MsgArgRootSetter::MsgArgRootSetter(DBusMessage* pMessage) noexcept { dbus_message_iter_init_append(pMessage, &self_); }

MsgArgRootGetter::MsgArgRootGetter(DBusMessage* pMessage) noexcept { dbus_message_iter_init(pMessage, &self_); }

}  // namespace tcap::dbus::_i
