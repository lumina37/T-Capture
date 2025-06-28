set(_TCAP_X11_SRCS
        include/tcap/screen/x11/display/box.cpp
        include/tcap/screen/x11/image/legacy.cpp
        include/tcap/screen/x11/image/shared.cpp
        include/tcap/screen/x11/screen.cpp
        include/tcap/screen/x11/window/box.cpp
        include/tcap/screen/x11/window/boxes.cpp
)
list(APPEND _TCAP_SRCS ${_TCAP_X11_SRCS})
