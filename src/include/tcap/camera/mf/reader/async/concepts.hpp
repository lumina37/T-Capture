#pragma once

#include <concepts>
#include <mutex>

#include <mfidl.h>
#include <mfreadwrite.h>

namespace tcap::mf {

template <typename Self>
concept CSampleCallback = true;

}  // namespace tcap::mf
