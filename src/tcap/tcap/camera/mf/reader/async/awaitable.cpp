#include <coroutine>
#include <expected>
#include <mutex>

#include "tcap/camera/mf/reader/async/awaitable.hpp"
#include "tcap/camera/mf/reader/async/callback.hpp"
#include "tcap/camera/mf/sample.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::mf {

void SampleAwaitable::await_suspend(std::coroutine_handle<> handle) noexcept {
    handle_ = handle;

    pCallback_->getMutex().lock();
    pCallback_->setCurrentAwaitable(this);
    auto sampleRes = pCallback_->getReaderBox().sampleAsync();
    pCallback_->getMutex().unlock();

    if (!sampleRes) {
        auto awaitLock = std::lock_guard{mutex_};
        setSampleBoxRes(std::unexpected{std::move(sampleRes.error())});
    }
}

std::expected<SampleBox, Error> SampleAwaitable::await_resume() noexcept {
    auto lock = std::lock_guard{mutex_};
    if (!sampleBoxRes_.has_value()) {
        return std::unexpected{std::move(sampleBoxRes_.error())};
    }
    return std::move(sampleBoxRes_.value());
}

}  // namespace tcap::mf
