#include <utility>

#include <dbus/dbus.h>

#include "tcap/screen/dbus/arg.hpp"
#include "tcap/screen/dbus/error.hpp"
#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/dbus/api/create_session/request.hpp"
#endif

namespace tcap::dbus {

ReqCreateSessionBox::ReqCreateSessionBox(DBusMessage* pMessage) noexcept : pMessage_(pMessage) {}

ReqCreateSessionBox::ReqCreateSessionBox(ReqCreateSessionBox&& rhs) noexcept
    : pMessage_(std::exchange(rhs.pMessage_, nullptr)) {}

ReqCreateSessionBox& ReqCreateSessionBox::operator=(ReqCreateSessionBox&& rhs) noexcept {
    pMessage_ = std::exchange(rhs.pMessage_, nullptr);
    return *this;
}

ReqCreateSessionBox::~ReqCreateSessionBox() noexcept {
    if (pMessage_ == nullptr) return;
    dbus_message_unref(pMessage_);
    pMessage_ = nullptr;
}

std::expected<ReqCreateSessionBox, Error> ReqCreateSessionBox::create(std::string_view sessionHandleToken) noexcept {
    ErrorBox errorBox;

    DBusMessage* pMessage =
        dbus_message_new_method_call("org.freedesktop.portal.Desktop", "/org/freedesktop/portal/desktop",
                                     "org.freedesktop.portal.ScreenCast", "CreateSession");
    if (pMessage == nullptr) {
        return std::unexpected{Error{ECate::eDBus, 0, "pMessage is nullptr"}};
    }

    _i::MsgArgRootSetter argRoot{pMessage};
    _i::MsgArgArray argArray{argRoot};
    _i::MsgArgEntrySetter argKey{argArray, "session_handle_token"};
    _i::MsgArgVariantSetter argValue{argKey, sessionHandleToken};

    return ReqCreateSessionBox{pMessage};
}

}  // namespace tcap::dbus
