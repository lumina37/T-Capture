#pragma once

#include <coroutine>
#include <expected>
#include <mutex>

#include "tcap/camera/mf/reader/async/callback.hpp"
#include "tcap/camera/mf/sample.hpp"
#include "tcap/common/defines.h"

namespace tcap::mf {

class SampleAwaitable {
public:
    TCAP_API SampleAwaitable(SampleCallback_<SampleAwaitable>* pCallback) noexcept
        : pCallback_(pCallback), sampleBoxRes_(std::unexpected{Error{}}) {}
    SampleAwaitable(const SampleAwaitable&) = delete;
    SampleAwaitable(SampleAwaitable&&) = delete;  // Pinned in memory

    bool await_ready() noexcept {
        return false;  // always suspend
    }
    // In this func we will call `ReadSample` to asynchronously request one frame
    void await_suspend(std::coroutine_handle<> handle) noexcept;
    // After resuming by the reader callback (`SampleCallback::OnReadSample`)
    // we return the `pSample_` as the result to the `co_await` caller
    std::expected<SampleBox, Error> await_resume() noexcept;

    std::mutex& getMutex() noexcept { return mutex_; }

    // This interface is for `SampleCallback::OnReadSample` to resume the coroutine
    void resume() noexcept { handle_.resume(); }
    // This interface is for `SampleCallback::OnReadSample` to set the `pSample_` as result
    void setSampleBoxRes(std::expected<SampleBox, Error>&& sampleBoxRes) noexcept {
        sampleBoxRes_ = std::move(sampleBoxRes);
    }

private:
    SampleCallback_<SampleAwaitable>* pCallback_;
    std::coroutine_handle<> handle_;
    std::mutex mutex_;

    // Result
    std::expected<SampleBox, Error> sampleBoxRes_;
};

}  // namespace tcap::mf
