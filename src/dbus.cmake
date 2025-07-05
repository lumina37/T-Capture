set(_TCAP_DBUS_SRCS
        include/tcap/screen/dbus/arg/array.cpp
        include/tcap/screen/dbus/arg/entry.cpp
        include/tcap/screen/dbus/arg/root.cpp
        include/tcap/screen/dbus/arg/string.cpp
        include/tcap/screen/dbus/arg/variant.cpp
        include/tcap/screen/dbus/api/create_session/request.cpp
        include/tcap/screen/dbus/api/create_session/response.cpp
        include/tcap/screen/dbus/connection.cpp
        include/tcap/screen/dbus/error.cpp
)
list(APPEND _TCAP_SRCS ${_TCAP_DBUS_SRCS})
