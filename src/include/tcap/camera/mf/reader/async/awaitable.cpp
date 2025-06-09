#include <coroutine>
#include <expected>

#include "tcap/camera/mf/reader/async/callback.hpp"
#include "tcap/camera/mf/sample.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader/async/awaitable.hpp"
#endif

namespace tcap::mf {

void SampleAwaitable::await_suspend(std::coroutine_handle<> handle) noexcept {
    handle_ = handle;
    pCallback_->setCurrentAwaitable(this);
    pCallback_->sampleNonBlock();  // would return immediately without blocking
}

std::expected<SampleBox, Error> SampleAwaitable::await_resume() noexcept {
    std::unique_lock lock(pCallback_->mutex_);
    if (pCallback_->err_.code != 0) {
        return std::unexpected{std::move(pCallback_->err_)};
    }
    SampleBox sampleBox = SampleBox::create(pSample_).value();
    return std::move(sampleBox);
}

}  // namespace tcap::mf
