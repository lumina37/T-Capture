#include <string>

#include <dbus/dbus.h>

#include "tcap/screen/dbus/arg/entry.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/dbus/arg/variant.hpp"
#endif

namespace tcap::dbus::_i {

MsgArgVariantSetter::MsgArgVariantSetter(MsgArgEntrySetter& parent, std::string_view value) noexcept : parent_(parent) {
    dbus_message_iter_open_container(parent.getPSelf(), DBUS_TYPE_VARIANT, DBUS_TYPE_STRING_AS_STRING, &self_);
    const auto pValue = value.data();
    dbus_message_iter_append_basic(&self_, DBUS_TYPE_STRING, &pValue);
}

MsgArgVariantSetter::~MsgArgVariantSetter() noexcept { dbus_message_iter_close_container(parent_.getPSelf(), &self_); }

}  // namespace tcap::dbus::_i
