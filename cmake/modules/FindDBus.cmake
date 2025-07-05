#.rst:
# FindDBus
# -------
#
# Try to find DBus on a Unix system.
#
# This will define the following variables:
#
# ``DBus_FOUND``
#     True if (the requested version of) DBus is available
# ``DBus_VERSION``
#     The version of DBus
#
# If ``DBus_FOUND`` is TRUE, it will also define the following imported target:
#
# ``DBus::DBus``
#     The DBus library

find_package(PkgConfig QUIET)

pkg_search_module(PKG_DBus QUIET dbus-1)

set(DBus_VERSION "${PKG_DBus_VERSION}")

find_library(DBus_LIBRARIES
        NAMES dbus-1
        HINTS ${PKG_DBus_LIBRARY_DIRS}
)

add_library(DBus-lib INTERFACE)
target_link_libraries(DBus-lib INTERFACE
        ${DBus_LIBRARIES}
)
target_compile_options(DBus-lib INTERFACE
        ${PKG_DBus_CFLAGS}
)

add_library(DBus::DBus ALIAS DBus-lib)
