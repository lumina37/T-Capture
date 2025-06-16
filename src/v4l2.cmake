set(_TCAP_MF_SRCS
        include/tcap/camera/v4l2/device/box.cpp
        include/tcap/camera/v4l2/device/path.cpp
        include/tcap/camera/v4l2/format/resolution.cpp
        include/tcap/camera/v4l2/format/box.cpp
        include/tcap/camera/v4l2/format/boxes.cpp
)
list(APPEND _TCAP_SRCS ${_TCAP_MF_SRCS})
