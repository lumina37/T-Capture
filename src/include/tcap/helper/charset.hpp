#pragma once

#include <expected>
#include <string>

#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::mf::_i {

[[nodiscard]] TCAP_API std::expected<std::string, Error> wstringToUtf8(std::wstring_view wstrView) noexcept;

}  // namespace tcap::mf::_i

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/helper/charset.cpp"
#endif
