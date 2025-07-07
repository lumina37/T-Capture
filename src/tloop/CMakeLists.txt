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

add_library(tloop-lib-header-only INTERFACE)
tloop_common_lib_setting(tloop-lib-header-only)
target_compile_definitions(tloop-lib-header-only INTERFACE "_TLOOP_LIB_HEADER_ONLY")
add_library(tloop::lib::header-only ALIAS tloop-lib-header-only)
