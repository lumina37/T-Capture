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

    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<> handle);
    std::expected<SampleBox, Error> await_resume();

    void resume() noexcept { handle_.resume(); }
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
