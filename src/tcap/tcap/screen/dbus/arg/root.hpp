#pragma once

#include <dbus/dbus.h>

namespace tcap::dbus::_i {

class MsgArgRootSetter {
public:
    MsgArgRootSetter(DBusMessage* pMessage) noexcept;

    [[nodiscard]] DBusMessageIter* getPSelf() noexcept { return &self_; }

private:
    DBusMessageIter self_;
};

class MsgArgRootGetter {
public:
    MsgArgRootGetter(DBusMessage* pMessage) noexcept;

    [[nodiscard]] DBusMessageIter* getPSelf() noexcept { return &self_; }

private:
    DBusMessageIter self_;
};

}  // namespace tcap::dbus::_i

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/dbus/arg/root.cpp"
#endif
