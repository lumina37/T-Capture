#pragma once

#include <string>

#include <dbus/dbus.h>

#include "tcap/screen/dbus/arg/array.hpp"

namespace tcap::dbus::_i {

class MsgArgEntrySetter {
public:
    MsgArgEntrySetter(MsgArgArray& parent, std::string_view key) noexcept;
    ~MsgArgEntrySetter() noexcept;

    [[nodiscard]] DBusMessageIter* getPSelf() noexcept { return &self_; }

private:
    MsgArgArray& parent_;
    DBusMessageIter self_;
};

}  // namespace tcap::dbus::_i
