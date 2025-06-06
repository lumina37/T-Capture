# FindMediaFoundation.cmake
# Find the Windows MediaFoundation SDK
#
# MediaFoundation::MediaFoundation - Exported target of the MediaFoundation
# MediaFoundation_FOUND - `TRUE` if MediaFoundation found

add_library(MediaFoundation-lib INTERFACE)

if (WIN32)
    target_link_libraries(MediaFoundation-lib INTERFACE
            mf.lib
            mfplat.lib
            mfreadwrite.lib
            mfuuid.lib
    )
    add_library(MediaFoundation::MediaFoundation ALIAS MediaFoundation-lib)
    set(MediaFoundation_FOUND TRUE)
endif ()

if (MediaFoundation_FOUND)
    if (NOT MediaFoundation_FIND_QUIETLY)
        message(STATUS "Found MediaFoundation")
    endif ()
else (MediaFoundation_FOUND)
    if (MediaFoundation_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find MediaFoundation")
    endif ()
endif (MediaFoundation_FOUND)
