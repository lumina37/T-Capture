#include <string>

#include <dbus/dbus.h>

#include "tcap/screen/dbus/arg/array.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/dbus/arg/entry.hpp"
#endif

namespace tcap::dbus::_i {

MsgArgEntrySetter::MsgArgEntrySetter(MsgArgArray& parent, std::string_view key) noexcept : parent_(parent) {
    dbus_message_iter_open_container(parent.getPSelf(), DBUS_TYPE_DICT_ENTRY, nullptr, &self_);
    const auto pKey = key.data();
    dbus_message_iter_append_basic(&self_, DBUS_TYPE_STRING, &pKey);
}

MsgArgEntrySetter::~MsgArgEntrySetter() noexcept { dbus_message_iter_close_container(parent_.getPSelf(), &self_); }

}  // namespace tcap::dbus::_i
