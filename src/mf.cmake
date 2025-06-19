set(_TCAP_MF_SRCS
        include/tcap/camera/mf/device/box.cpp
        include/tcap/camera/mf/device/boxes.cpp
        include/tcap/camera/mf/reader/core.cpp
        include/tcap/camera/mf/reader/async/awaitable.cpp
        include/tcap/camera/mf/reader/async/reader.cpp
        include/tcap/camera/mf/reader/sync/reader.cpp
        include/tcap/camera/mf/buffer.cpp
        include/tcap/camera/mf/media_type.cpp
        include/tcap/camera/mf/reader_type.cpp
        include/tcap/camera/mf/sample.cpp
        include/tcap/camera/mf/source.cpp

        include/tcap/helper/mf/attributes.cpp

        include/tcap/utils/call_once/mf/detail.cpp
)
list(APPEND _TCAP_SRCS ${_TCAP_MF_SRCS})
