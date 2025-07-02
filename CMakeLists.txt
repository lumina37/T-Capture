cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(tcap
        VERSION 0.0.1
        DESCRIPTION "Cross-platform screen / camera capture library"
        LANGUAGES CXX
)

set(TCAP_VERSION_BETA_SUFFIX "alpha.0")
if (NOT TCAP_VERSION_BETA_SUFFIX)
    set(TCAP_VERSION ${PROJECT_VERSION})
else ()
    set(TCAP_VERSION ${PROJECT_VERSION}-${TCAP_VERSION_BETA_SUFFIX})
endif ()

# Options
option(TCAP_ENABLE_TESTS "Enable tests" OFF)
option(TCAP_VERBOSE_WARNING "Show verbose compiler warnings" OFF)

# Dependencies
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

if (WIN32)
    find_package(MediaFoundation)
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(X11)
    find_package(PipeWire)
endif ()

include(FetchContent)

if (TCAP_ENABLE_TESTS)
    include(CTest)

    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.8.0
    )
    FetchContent_MakeAvailable(Catch2)
    include(Catch)
endif ()

# Helper
function(tcap_copy_dlls_if_needed name)
    if (WIN32)
        add_custom_command(TARGET ${name} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                copy_if_different $<TARGET_RUNTIME_DLLS:${name}> $<TARGET_FILE:${name}> $<TARGET_FILE_DIR:${name}> COMMAND_EXPAND_LISTS)
    endif ()
endfunction()

# Sources
add_subdirectory(src)
add_subdirectory(samples)

if (TCAP_ENABLE_TESTS)
    add_subdirectory(tests)
endif ()
