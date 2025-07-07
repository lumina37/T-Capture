#.rst:
# FindMediaFoundation
# -------
#
# Try to find the Windows MediaFoundation SDK.
#
# This will define the following variables:
#
# ``MediaFoundation_FOUND``
#     True if (the requested version of) Media Foundation is available
# ``MediaFoundation_LIBRARIES``
#     This can be passed to target_link_libraries() instead of the ``MediaFoundation::MediaFoundation``
#     target
#
# If ``MediaFoundation_FOUND`` is TRUE, it will also define the following imported target:
#
# ``MediaFoundation::MediaFoundation``
#     The Media Foundation library

add_library(MediaFoundation-lib INTERFACE)

if (NOT WIN32)
    set(MediaFoundation_FOUND FALSE)
    return()
endif ()

set(MediaFoundation_LIBRARIES
        mf
        mfplat
        mfreadwrite
        mfuuid
        shlwapi
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MediaFoundation
        FOUND_VAR MediaFoundation_FOUND
        REQUIRED_VARS MediaFoundation_LIBRARIES
)

add_library(MediaFoundation::MediaFoundation INTERFACE IMPORTED)
set_target_properties(MediaFoundation::MediaFoundation PROPERTIES
        INTERFACE_LINK_LIBRARIES "${MediaFoundation_LIBRARIES}"
)
set(MediaFoundation_FOUND TRUE)
