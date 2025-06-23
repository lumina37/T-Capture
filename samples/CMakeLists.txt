function(vkc_add_sample name src)
    add_executable(${name} ${src})

    target_link_libraries(${name} PRIVATE
            tcap::lib::static
    )

    tcap_copy_dlls_if_needed(${name})
endfunction()

if (WIN32)
    vkc_add_sample(tcap-camera-mf-async camera/mf/async.cpp)
    vkc_add_sample(tcap-camera-mf-sync camera/mf/sync.cpp)
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    vkc_add_sample(tcap-camera-v4l2-mmap camera/v4l2/mmap.cpp)

    vkc_add_sample(tcap-screen-x11-legacy screen/x11/legacy.cpp)
endif ()
