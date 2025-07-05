#pragma once

#include <string>

#include "tcap/screen/dbus/arg/root.hpp"

namespace tcap::dbus::_i {

class MsgArgStrGetter {
public:
    MsgArgStrGetter(MsgArgRootGetter& parent) noexcept;

    [[nodiscard]] std::string_view get() const noexcept { return data_; }

private:
    std::string_view data_;
};

}  // namespace tcap::dbus::_i

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/dbus/arg/entry.cpp"
#endif
