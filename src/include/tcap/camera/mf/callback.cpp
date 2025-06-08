#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/callback.hpp"
#endif

namespace tcap::mf {

SampleCallback::SampleCallback(SampleCallback&& rhs) noexcept
    : pReader_(std::exchange(rhs.pReader_, nullptr)),
      currentAwaitable_(std::exchange(rhs.currentAwaitable_, nullptr)),
      refCount_(rhs.refCount_),
      err_(std::move(rhs.err_)) {}

}  // namespace tcap::mf
