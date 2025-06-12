function(vkc_add_sample name src)
    add_executable(${name} ${src})

    target_link_libraries(${name} PRIVATE
            tcap::lib::static
    )

    tcap_copy_dlls_if_needed(${name})
endfunction()

if (WIN32)
    vkc_add_sample(tcap-mf-camera-async mf/camera/async.cpp)
    vkc_add_sample(tcap-mf-camera-sync mf/camera/sync.cpp)
endif ()
