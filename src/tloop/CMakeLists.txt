include(cmake/helper.cmake)

# Sources
set(_TLOOP_SRCS
        tloop/utils/error.cpp
)

if (WIN32)
    include(cmake/iocp.cmake)
endif ()

# Libraries
add_library(tloop-lib-static STATIC ${_TLOOP_SRCS})
tloop_common_lib_setting(tloop-lib-static)
add_library(tloop::lib::static ALIAS tloop-lib-static)

add_library(tloop-lib-dynamic SHARED ${_TLOOP_SRCS})
tloop_common_lib_setting(tloop-lib-dynamic)
set_target_properties(tloop-lib-dynamic PROPERTIES
        OUTPUT_NAME tloop
)
target_compile_definitions(tloop-lib-dynamic PUBLIC "_TLOOP_LIB_DYNAMIC")
add_library(tloop::lib::dynamic ALIAS tloop-lib-dynamic)
