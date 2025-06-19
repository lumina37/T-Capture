#pragma once

#include <concepts>
#include <mutex>

#include "tcap/camera/mf/reader/core.hpp"

namespace tcap::mf {

template <typename Self>
concept CSampleAwaitable = requires(Self self) {
    { self.resume() };
    { self.getMutex() } -> std::same_as<std::mutex&>;
} && requires(Self self, std::expected<SampleBox, Error>&& sampleBoxRes) {
    { self.setSampleBoxRes(std::move(sampleBoxRes)) };
};

}  // namespace tcap::mf
