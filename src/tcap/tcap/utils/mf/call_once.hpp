#pragma once

#include <expected>

#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::mf {

[[nodiscard]] TCAP_API std::expected<void, Error> globalInit() noexcept;

TCAP_API void globalDestroy() noexcept;

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/utils/mf/call_once.cpp"
#endif
