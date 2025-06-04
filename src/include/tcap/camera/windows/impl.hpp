#pragma once

namespace tcap {

void EnumerateCaptureDevices();

}

#ifdef _TLCT_LIB_HEADER_ONLY
#    include "tcap/camera/windows/impl.cpp"
#endif
