function(tcap_eval_dep_scope lib dep_scope)
    get_target_property(__TYPE ${lib} TYPE)
    if (${__TYPE} STREQUAL "INTERFACE_LIBRARY")
        set(${dep_scope} "INTERFACE" PARENT_SCOPE)
    else ()
        set(${dep_scope} "PUBLIC" PARENT_SCOPE)
    endif ()
endfunction()

function(tcap_common_lib_setting lib)
    tcap_eval_dep_scope(${lib} __PUB_DEP_SCOPE)

    target_compile_features(${lib} ${__PUB_DEP_SCOPE} cxx_std_23)

    target_include_directories(${lib} ${__PUB_DEP_SCOPE}
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    if (WIN32)
        target_link_libraries(${lib} ${__PUB_DEP_SCOPE}
                MediaFoundation::MediaFoundation
        )
    endif ()

    if (MSVC)
        target_compile_options(${lib} ${__PUB_DEP_SCOPE} /fsanitize=address)
    endif ()

    if (TCAP_VERBOSE_WARNING)
        if (MSVC)
            target_compile_options(${lib} ${__PUB_DEP_SCOPE} /W4)
        else ()
            target_compile_options(${lib} ${__PUB_DEP_SCOPE} -Wall -Wextra)
        endif ()
    endif ()
endfunction()

file(GLOB_RECURSE _TCAP_SRCS "include/*.cpp")

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
