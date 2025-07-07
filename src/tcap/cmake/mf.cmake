set(_TCAP_MF_SRCS
        tcap/camera/mf/device/box.cpp
        tcap/camera/mf/device/boxes.cpp
        tcap/camera/mf/reader/core.cpp
        tcap/camera/mf/reader/async/awaitable.cpp
        tcap/camera/mf/reader/async/reader.cpp
        tcap/camera/mf/reader/sync/reader.cpp
        tcap/camera/mf/buffer.cpp
        tcap/camera/mf/media_type.cpp
        tcap/camera/mf/reader_type.cpp
        tcap/camera/mf/sample.cpp
        tcap/camera/mf/source.cpp

        tcap/helper/mf/attributes.cpp

        tcap/utils/mf/call_once.cpp
        tcap/utils/mf/wstring.cpp
)
list(APPEND _TCAP_SRCS ${_TCAP_MF_SRCS})
