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
