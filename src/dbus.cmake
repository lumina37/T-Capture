set(_TCAP_DBUS_SRCS
        include/tcap/screen/dbus/message/create_session/request.cpp
        include/tcap/screen/dbus/message/create_session/response.cpp
        include/tcap/screen/dbus/connection.cpp
        include/tcap/screen/dbus/error.cpp
)
list(APPEND _TCAP_SRCS ${_TCAP_DBUS_SRCS})
