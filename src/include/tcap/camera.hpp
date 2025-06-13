// Capture from camera

#pragma once

#include "tcap/common/defines.h"

#ifdef TCAP_ENABLE_MF
#    include "tcap/camera/mf.hpp"
#endif

#ifdef TCAP_ENABLE_V4L2
#    include "tcap/camera/v4l2.hpp"
#endif
