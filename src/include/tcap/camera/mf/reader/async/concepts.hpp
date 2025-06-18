#pragma once

#include <concepts>
#include <mutex>

#include <mfidl.h>
#include <mfreadwrite.h>

namespace tcap::mf {

template <typename Self>
concept CSampleCallback = requires(Self self, IMFSourceReader* pReader) {
    { self.setPReader(pReader) };
    { self.autoLock() } -> std::same_as<std::unique_lock<std::mutex>>;
};

}  // namespace tcap::mf
