#pragma once

#include <dbus/dbus.h>

#include "tcap/screen/dbus/arg/root.hpp"

namespace tcap::dbus::_i {

class MsgArgArray {
public:
    MsgArgArray(MsgArgRootSetter& parent) noexcept;
    ~MsgArgArray() noexcept;

    [[nodiscard]] DBusMessageIter* getPSelf() noexcept { return &self_; }

private:
    MsgArgRootSetter& parent_;
    DBusMessageIter self_;
};

}  // namespace tcap::dbus::_i
