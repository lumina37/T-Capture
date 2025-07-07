set(_TCAP_V4L2_SRCS
        tcap/camera/v4l2/buffer/mmap.cpp
        tcap/camera/v4l2/device/box.cpp
        tcap/camera/v4l2/device/caps.cpp
        tcap/camera/v4l2/device/path.cpp
        tcap/camera/v4l2/format/active.cpp
        tcap/camera/v4l2/format/fps.cpp
        tcap/camera/v4l2/format/native.cpp
        tcap/camera/v4l2/format/resolution.cpp
        tcap/camera/v4l2/queue/caps.cpp
        tcap/camera/v4l2/queue/mmap.cpp
        tcap/camera/v4l2/sample/mmap.cpp
)
list(APPEND _TCAP_SRCS ${_TCAP_V4L2_SRCS})
