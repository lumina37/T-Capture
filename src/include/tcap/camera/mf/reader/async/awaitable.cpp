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
    if (!sampleBoxRes_.has_value()) {
        return std::unexpected{std::move(sampleBoxRes_.error())};
    }
    return std::move(sampleBoxRes_.value());
}

}  // namespace tcap::mf
