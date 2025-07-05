#pragma once

#include <expected>

#include <dbus/dbus.h>

#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::dbus {

class ConnectionBox {
    ConnectionBox(DBusConnection* pConn) noexcept;

public:
    ConnectionBox& operator=(const ConnectionBox&) = delete;
    ConnectionBox(const ConnectionBox&) = delete;
    TCAP_API ConnectionBox(ConnectionBox&& rhs) noexcept;
    TCAP_API ConnectionBox& operator=(ConnectionBox&& rhs) noexcept;
    TCAP_API ~ConnectionBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<ConnectionBox, Error> create() noexcept;

    [[nodiscard]] TCAP_API DBusConnection* getPConnection() noexcept { return pConn_; }

private:
    DBusConnection* pConn_;
};

}  // namespace tcap::dbus

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/dbus/connection.cpp"
#endif
