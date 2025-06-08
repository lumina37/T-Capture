#pragma once

#include <coroutine>
#include <expected>

#include <mfobjects.h>

#include "tcap/camera/mf/sample.hpp"

namespace tcap::mf {

class SampleCallback;

class SampleAwaitable {
public:
    SampleAwaitable(SampleCallback* pCallback) noexcept : pCallback_(pCallback) {}
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

    // This interface is for `SampleCallback::OnReadSample` to resume the coroutine
    void resume() noexcept { handle_.resume(); }
    // This interface is for `SampleCallback::OnReadSample` to set the `pSample_` as result
    void setPSample(IMFSample* pSample) noexcept { pSample_ = pSample; }

private:
    SampleCallback* pCallback_;
    std::coroutine_handle<> handle_;

    // Result
    IMFSample* pSample_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/coroutine.cpp"
#endif
