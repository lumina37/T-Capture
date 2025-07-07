#pragma once

#include <string>

#include <dbus/dbus.h>

#include "tcap/screen/dbus/arg/entry.hpp"

namespace tcap::dbus::_i {

class MsgArgVariantSetter {
public:
    MsgArgVariantSetter(MsgArgEntrySetter& parent, std::string_view value) noexcept;
    ~MsgArgVariantSetter() noexcept;

    [[nodiscard]] DBusMessageIter* getPSelf() noexcept { return &self_; }

private:
    MsgArgEntrySetter& parent_;
    DBusMessageIter self_;
};

}  // namespace tcap::dbus::_i

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/dbus/arg/variant.cpp"
#endif
