#include <string>

#include <dbus/dbus.h>

#include "tcap/screen/dbus/arg/root.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/dbus/arg/string.hpp"
#endif

namespace tcap::dbus::_i {

MsgArgStrGetter::MsgArgStrGetter(MsgArgRootGetter& parent) noexcept {
    const char* pData;
    dbus_message_iter_get_basic(parent.getPSelf(), &pData);
    data_ = std::string_view{pData};
}

}  // namespace tcap::dbus::_i
