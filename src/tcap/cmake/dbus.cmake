set(_TCAP_DBUS_SRCS
        tcap/screen/dbus/arg/array.cpp
        tcap/screen/dbus/arg/entry.cpp
        tcap/screen/dbus/arg/root.cpp
        tcap/screen/dbus/arg/string.cpp
        tcap/screen/dbus/arg/variant.cpp
        tcap/screen/dbus/api/create_session/request.cpp
        tcap/screen/dbus/api/create_session/response.cpp
        tcap/screen/dbus/connection.cpp
        tcap/screen/dbus/error.cpp
)
list(APPEND _TCAP_SRCS ${_TCAP_DBUS_SRCS})
