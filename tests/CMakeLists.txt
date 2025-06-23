enable_testing()

set(_TCAP_TEST_TARGETS)

function(tcap_add_test bin lib srcs)
    add_executable(${bin} ${srcs})
    target_link_libraries(${bin} PRIVATE Catch2::Catch2WithMain ${lib})

    catch_discover_tests(${bin})
    list(APPEND _TCAP_TEST_TARGETS ${bin})
    set(_TCAP_TEST_TARGETS ${_TCAP_TEST_TARGETS} PARENT_SCOPE)

    tcap_copy_dlls_if_needed(${bin})
endfunction()

if (WIN32)
    tcap_add_test(test-mf-camera-async tcap::lib::static "camera/mf/async.cpp")
    tcap_add_test(test-mf-camera-sync tcap::lib::static "camera/mf/sync.cpp")

    tcap_add_test(test_mf_camera_async_header_only_lib tcap::lib::header-only "camera/mf/async.cpp")
    tcap_add_test(test-mf-camera-sync-header_only_lib tcap::lib::header-only "camera/mf/sync.cpp")
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    tcap_add_test(test-v4l2-camera-mmap tcap::lib::static "camera/v4l2/mmap.cpp")
    tcap_add_test(test-x11-screen-legacy tcap::lib::static "screen/x11/legacy.cpp")

    tcap_add_test(test-v4l2-camera-mmap-header_only_lib tcap::lib::header-only "camera/v4l2/mmap.cpp")
    tcap_add_test(test-x11-screen-legacy-header_only_lib tcap::lib::header-only "screen/x11/legacy.cpp")
endif ()

add_custom_target(tcap-tests DEPENDS ${_TCAP_TEST_TARGETS})
