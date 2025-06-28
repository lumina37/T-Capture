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

    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_link_libraries(${lib} ${__PUB_DEP_SCOPE}
                X11::X11
                X11::Xext
        )
    endif ()

    if (TCAP_VERBOSE_WARNING)
        if (MSVC)
            target_compile_options(${lib} ${__PUB_DEP_SCOPE} /W4)
        else ()
            target_compile_options(${lib} ${__PUB_DEP_SCOPE} -Wall -Wextra)
        endif ()
    endif ()
endfunction()
