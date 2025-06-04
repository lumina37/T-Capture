#pragma once

#include <expected>
#include <string>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/helper/charset.hpp"
#endif

namespace tcap {

std::expected<std::string, Error> wstringToUtf8(const std::wstring_view wstrView) noexcept {
    int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wstrView.data(), (int)wstrView.size(), nullptr, 0, nullptr, nullptr);
    if (utf8Size == 0) {
        return std::unexpected{Error{-1, "wstringToLocal failed"}};
    }
    std::string utf8Str(utf8Size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstrView.data(), (int)wstrView.size(), utf8Str.data(), utf8Size, nullptr, nullptr);
    return utf8Str;
}

}  // namespace tcap
