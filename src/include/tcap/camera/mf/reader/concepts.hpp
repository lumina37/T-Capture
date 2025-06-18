#pragma once

#include <concepts>

#include <mfobjects.h>

namespace tcap::mf {

template <typename Self>
concept CSupportGetPReader = requires(Self self) {
    { self.getPReader() } -> std::same_as<IMFSourceReader*>;
};

}  // namespace tcap::mf
