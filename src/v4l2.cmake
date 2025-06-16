set(_TCAP_V4L2_SRCS
        include/tcap/camera/v4l2/device/box.cpp
        include/tcap/camera/v4l2/device/path.cpp
        include/tcap/camera/v4l2/format/native.cpp
        include/tcap/camera/v4l2/format/fps.cpp
        include/tcap/camera/v4l2/format/resolution.cpp
)
list(APPEND _TCAP_SRCS ${_TCAP_V4L2_SRCS})
