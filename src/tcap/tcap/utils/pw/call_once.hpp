#pragma once

#include <expected>

#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::pw {

[[nodiscard]] TCAP_API std::expected<void, Error> globalInit() noexcept;

TCAP_API void globalDestroy() noexcept;

}  // namespace tcap::pw
