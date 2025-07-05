#include <utility>

#include <dbus/dbus.h>

#include "tcap/screen/dbus/error.hpp"
#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/dbus/message/create_session/request.hpp"
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

    DBusMessageIter itRoot;
    dbus_message_iter_init_append(pMessage, &itRoot);

    DBusMessageIter itOptions;
    dbus_message_iter_open_container(&itRoot, DBUS_TYPE_ARRAY, "{sv}", &itOptions);

    DBusMessageIter itSessionHandleTokenEntry;
    dbus_message_iter_open_container(&itOptions, DBUS_TYPE_DICT_ENTRY, nullptr, &itSessionHandleTokenEntry);
    const char* sessionHandleTokenKey = "session_handle_token";
    dbus_message_iter_append_basic(&itSessionHandleTokenEntry, DBUS_TYPE_STRING, &sessionHandleTokenKey);

    DBusMessageIter itSessionHandleTokenVar;
    dbus_message_iter_open_container(&itSessionHandleTokenEntry, DBUS_TYPE_VARIANT, "s", &itSessionHandleTokenVar);
    auto pSessionHandleToken = sessionHandleToken.data();
    dbus_message_iter_append_basic(&itSessionHandleTokenVar, DBUS_TYPE_STRING, &pSessionHandleToken);
    dbus_message_iter_close_container(&itSessionHandleTokenEntry, &itSessionHandleTokenVar);

    dbus_message_iter_close_container(&itOptions, &itSessionHandleTokenEntry);

    dbus_message_iter_close_container(&itRoot, &itOptions);

    return ReqCreateSessionBox{pMessage};
}

}  // namespace tcap::dbus
