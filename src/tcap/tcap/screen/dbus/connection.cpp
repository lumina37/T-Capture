#include <utility>

#include <dbus/dbus.h>

#include "tcap/screen/dbus/api/create_session.hpp"
#include "tcap/screen/dbus/error.hpp"
#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/dbus/connection.hpp"
#endif

namespace tcap::dbus {

ConnectionBox::ConnectionBox(DBusConnection* pConn) noexcept : pConn_(pConn) {}

ConnectionBox::ConnectionBox(ConnectionBox&& rhs) noexcept : pConn_(std::exchange(rhs.pConn_, nullptr)) {}

ConnectionBox& ConnectionBox::operator=(ConnectionBox&& rhs) noexcept {
    pConn_ = std::exchange(rhs.pConn_, nullptr);
    return *this;
}

ConnectionBox::~ConnectionBox() noexcept {
    if (pConn_ == nullptr) return;
    dbus_connection_unref(pConn_);
    pConn_ = nullptr;
}

std::expected<ConnectionBox, Error> ConnectionBox::create() noexcept {
    ErrorBox errorBox;

    DBusConnection* pConn = dbus_bus_get(DBUS_BUS_SESSION, errorBox.getPError());
    if (errorBox.hasError()) {
        return std::unexpected{errorBox.toError()};
    }
    if (pConn == nullptr) {
        return std::unexpected{Error{ECate::eDBus, 0, "pConn is nullptr"}};
    }

    return ConnectionBox{pConn};
}

std::expected<ResCreateSessionBox, Error> ConnectionBox::createSession(ReqCreateSessionBox&& request) noexcept {
    ErrorBox errorBox;

    DBusMessage* pRes = dbus_connection_send_with_reply_and_block(pConn_, request.getPMessage(),
                                                                  DBUS_TIMEOUT_USE_DEFAULT, errorBox.getPError());
    if (errorBox.hasError()) return std::unexpected{errorBox.toError()};

    auto responseRes = ResCreateSessionBox::create(pRes);
    ResCreateSessionBox& response = responseRes.value();

    return std::move(response);
}

}  // namespace tcap::dbus
