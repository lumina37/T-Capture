include(cmake/helper.cmake)

# Sources
set(_TCAP_SRCS
        tcap/helper/charset.cpp
)

if (WIN32)
    include(cmake/mf.cmake)
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    include(cmake/v4l2.cmake)
    include(cmake/x11.cmake)
    include(cmake/dbus.cmake)
    include(cmake/pw.cmake)
endif ()

# Libraries
add_library(tcap-lib-static STATIC ${_TCAP_SRCS})
tcap_common_lib_setting(tcap-lib-static)
add_library(tcap::lib::static ALIAS tcap-lib-static)

add_library(tcap-lib-dynamic SHARED ${_TCAP_SRCS})
tcap_common_lib_setting(tcap-lib-dynamic)
set_target_properties(tcap-lib-dynamic PROPERTIES
        OUTPUT_NAME tcap
        DEBUG_POSTFIX "-${TCAP_VERSION}d"
        RELEASE_POSTFIX "-${TCAP_VERSION}"
)
target_compile_definitions(tcap-lib-dynamic PUBLIC "_TCAP_LIB_DYNAMIC")
add_library(tcap::lib::dynamic ALIAS tcap-lib-dynamic)

add_library(tcap-lib-header-only INTERFACE)
tcap_common_lib_setting(tcap-lib-header-only)
target_compile_definitions(tcap-lib-header-only INTERFACE "_TCAP_LIB_HEADER_ONLY")
add_library(tcap::lib::header-only ALIAS tcap-lib-header-only)
