#pragma once

#include <coroutine>
#include <expected>

#include "tcap/camera/mf/reader/async/concepts.hpp"
#include "tcap/camera/mf/sample.hpp"
#include "tcap/common/defines.h"

namespace tcap::mf {

class SampleCallback;

template <typename TSampleCallback_>
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
    void await_suspend(std::coroutine_handle<> handle) noexcept {
        handle_ = handle;
        pCallback_->setCurrentAwaitable(this);
        pCallback_->sampleNonBlock();  // would return immediately without blocking
    }
    // After resuming by the reader callback (`SampleCallback::OnReadSample`)
    // we return the `pSample_` as the result to the `co_await` caller
    std::expected<SampleBox, Error> await_resume() noexcept {
        auto lock = pCallback_->autoLock();
        if (!sampleBoxRes_.has_value()) {
            return std::unexpected{std::move(sampleBoxRes_.error())};
        }
        return std::move(sampleBoxRes_.value());
    }

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

    // Result
    std::expected<SampleBox, Error> sampleBoxRes_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader/async/awaitable.cpp"
#endif
