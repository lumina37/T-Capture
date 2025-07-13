#pragma once

#include "tloop/common/defines.h"

namespace tlp::iocp {

class EventLoop {};

}  // namespace tlp::iocp

#ifdef _TLOOP_LIB_HEADER_ONLY
#    include "tloop/iocp/loop.cpp"
#endif
