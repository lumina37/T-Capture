#include <coroutine>
#include <expected>

#include "tcap/camera/mf/callback.hpp"
#include "tcap/camera/mf/sample.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/coroutine.hpp"
#endif

namespace tcap::mf {

void SampleAwaitable::await_suspend(std::coroutine_handle<> handle) {
    handle_ = handle;
    pCallback_->setCurrentAwaitable(this);
    pCallback_->_sample();
}

std::expected<SampleBox, Error> SampleAwaitable::await_resume() {
    if (pCallback_->err_.code != 0) {
        return std::unexpected{pCallback_->err_};
    }
    SampleBox sampleBox = SampleBox::create(pSample_).value();
    return std::move(sampleBox);
}

}  // namespace tcap::mf
