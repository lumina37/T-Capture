// Capture from screen

#pragma once

#include "tcap/common/defines.h"

#if TCAP_ENABLE_X11
#    include "tcap/screen/x11.hpp"
#endif

#if TCAP_ENABLE_PW
#    include "tcap/screen/dbus.hpp"
#endif
