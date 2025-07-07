set(_TCAP_X11_SRCS
        tcap/screen/x11/display/box.cpp
        tcap/screen/x11/display/caps.cpp
        tcap/screen/x11/image/legacy.cpp
        tcap/screen/x11/image/shared.cpp
        tcap/screen/x11/screen.cpp
        tcap/screen/x11/window/box.cpp
        tcap/screen/x11/window/boxes.cpp
)
list(APPEND _TCAP_SRCS ${_TCAP_X11_SRCS})
