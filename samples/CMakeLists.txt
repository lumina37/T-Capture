function(vkc_add_sample name src)
    add_executable(${name} ${src})

    target_link_libraries(${name} PRIVATE
            tcap::lib::static
    )

    tcap_copy_dlls_if_needed(${name})
endfunction()

vkc_add_sample(tcap-camera-capture-sync camera-capture-sync.cpp)
vkc_add_sample(tcap-camera-capture-async camera-capture-async.cpp)
