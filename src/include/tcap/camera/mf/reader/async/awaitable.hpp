#pragma once

#include <coroutine>
#include <expected>
#include <mutex>

#include "tcap/camera/mf/reader/async/concepts.hpp"
#include "tcap/camera/mf/sample.hpp"
#include "tcap/common/defines.h"

namespace tcap::mf {

class SampleCallback;

template <CSampleCallback TSampleCallback_>
class SampleAwaitable_ {
public:
    using TSampleCallback = TSampleCallback_;

    TCAP_API SampleAwaitable_(TSampleCallback* pCallback) noexcept
        : pCallback_(pCallback), sampleBoxRes_(std::unexpected{Error{}}) {}
    SampleAwaitable_(const SampleAwaitable_&) = delete;
    SampleAwaitable_(SampleAwaitable_&&) = delete;  // Pinned in memory

    bool await_ready() noexcept {
        return false;  // always suspend
    }
    // In this func we will call `ReadSample` to asynchronously request one frame
    void await_suspend(std::coroutine_handle<> handle) noexcept;
    // After resuming by the reader callback (`SampleCallback::OnReadSample`)
    // we return the `pSample_` as the result to the `co_await` caller
    std::expected<SampleBox, Error> await_resume() noexcept;

    auto autoLock() noexcept { return std::lock_guard{mutex_}; }

    friend class SampleCallback;

private:
    // This interface is for `SampleCallback::OnReadSample` to resume the coroutine
    void resume() noexcept { handle_.resume(); }
    // This interface is for `SampleCallback::OnReadSample` to set the `pSample_` as result
    void setSampleBoxRes(std::expected<SampleBox, Error>&& sampleBoxRes) noexcept {
        sampleBoxRes_ = std::move(sampleBoxRes);
    }

    TSampleCallback* pCallback_;
    std::coroutine_handle<> handle_;
    std::mutex mutex_;

    // Result
    std::expected<SampleBox, Error> sampleBoxRes_;
};

template <CSampleCallback TSampleCallback>
void SampleAwaitable_<TSampleCallback>::await_suspend(std::coroutine_handle<> handle) noexcept {
    handle_ = handle;

    pCallback_->lock();
    pCallback_->setCurrentAwaitable(this);
    auto sampleRes = pCallback_->getReaderBox().sampleAsync();
    pCallback_->unlock();

    if (!sampleRes) {
        auto lock = autoLock();
        setSampleBoxRes(std::unexpected{std::move(sampleRes.error())});
    }
}

template <CSampleCallback TSampleCallback>
std::expected<SampleBox, Error> SampleAwaitable_<TSampleCallback>::await_resume() noexcept {
    auto lock = autoLock();
    if (!sampleBoxRes_.has_value()) {
        return std::unexpected{std::move(sampleBoxRes_.error())};
    }
    return std::move(sampleBoxRes_.value());
}

}  // namespace tcap::mf
