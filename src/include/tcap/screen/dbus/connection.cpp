#include <utility>

#include <dbus/dbus.h>

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

}  // namespace tcap::dbus
