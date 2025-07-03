#.rst:
# FindPipeWire
# -------
#
# Try to find PipeWire on a Unix system.
#
# This will define the following variables:
#
# ``PipeWire_FOUND``
#     True if (the requested version of) PipeWire is available
# ``PipeWire_VERSION``
#     The version of PipeWire
#
# If ``PipeWire_FOUND`` is TRUE, it will also define the following imported target:
#
# ``PipeWire::PipeWire``
#     The PipeWire library


find_package(PkgConfig QUIET)

pkg_search_module(PKG_PipeWire QUIET libpipewire-0.3)
pkg_search_module(PKG_Spa QUIET libspa-0.2)
pkg_search_module(PKG_DBus QUIET dbus-1)

set(PipeWire_VERSION "${PKG_PipeWire_VERSION}")

find_library(PipeWire_LIBRARIES
        NAMES pipewire-0.3
        HINTS ${PKG_PipeWire_LIBRARY_DIRS}
)

find_library(DBus_LIBRARIES
        NAMES dbus-1
        HINTS ${PKG_DBus_LIBRARY_DIRS}
)

add_library(PipeWire-lib INTERFACE)
target_link_libraries(PipeWire-lib INTERFACE
        ${PipeWire_LIBRARIES}
        ${DBus_LIBRARIES}
)
target_compile_options(PipeWire-lib INTERFACE
        ${PKG_PipeWire_CFLAGS}
        ${PKG_Spa_CFLAGS}
        ${PKG_DBus_CFLAGS}
)

add_library(PipeWire::PipeWire ALIAS PipeWire-lib)
