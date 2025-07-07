#include <expected>
#include <string>

#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/helper/charset.hpp"
#endif

namespace tcap::_i {

constexpr bool isHiSurrogate(const wchar_t wc) { return 0xD800 <= wc && wc <= 0xDBFF; }

constexpr bool isLoSurrogate(const wchar_t wc) { return 0xDC00 <= wc && wc <= 0xDFFF; }

constexpr char32_t surrogatePair2Codepoint(const wchar_t hi, const wchar_t lo) {
    return ((static_cast<char32_t>(hi) - 0xD800) << 10) + (static_cast<char32_t>(lo) - 0xDC00) + 0x10000;
}

std::expected<std::string, Error> wstringToUtf8(std::wstring_view wstrView) noexcept {
    std::string utf8Str;
    utf8Str.reserve(wstrView.size());

    for (size_t chIdx = 0; chIdx < wstrView.size(); chIdx++) {
        char32_t codepoint;

        const wchar_t wideCh = wstrView[chIdx];

        // Convert to codepoint
        if constexpr (sizeof(wchar_t) == 2) {
            if (isHiSurrogate(wideCh)) {
                if (chIdx + 1 >= wstrView.size() || !isLoSurrogate(wstrView[chIdx + 1])) {
                    return std::unexpected{Error{ECate::eTCap, ECode::eUnexValue, "invalid surrogate pair"}};
                }
                codepoint = surrogatePair2Codepoint(wideCh, wstrView[++chIdx]);
            } else if (isLoSurrogate(wideCh)) {
                return std::unexpected{Error{ECate::eTCap, ECode::eUnexValue, "unpaired low surrogate"}};
            } else {
                codepoint = (char32_t)wideCh;
            }
        } else if constexpr (sizeof(wchar_t) == 4) {
            codepoint = (char32_t)wideCh;
        } else {
            return std::unexpected{Error{ECate::eTCap, ECode::eNoImpl, "unsupported wchar_t size"}};
        }

        if (codepoint <= 0x7F) {
            utf8Str.push_back((char)codepoint);
        } else if (codepoint <= 0x7FF) {
            utf8Str.push_back((char)(0xC0 | ((codepoint >> 6) & 0x1F)));
            utf8Str.push_back((char)(0x80 | (codepoint & 0x3F)));
        } else if (codepoint <= 0xFFFF) {
            utf8Str.push_back((char)(0xE0 | ((codepoint >> 12) & 0x0F)));
            utf8Str.push_back((char)(0x80 | ((codepoint >> 6) & 0x3F)));
            utf8Str.push_back((char)(0x80 | (codepoint & 0x3F)));
        } else if (codepoint <= 0x10FFFF) {
            utf8Str.push_back((char)(0xF0 | ((codepoint >> 18) & 0x07)));
            utf8Str.push_back((char)(0x80 | ((codepoint >> 12) & 0x3F)));
            utf8Str.push_back((char)(0x80 | ((codepoint >> 6) & 0x3F)));
            utf8Str.push_back((char)(0x80 | (codepoint & 0x3F)));
        } else {
            return std::unexpected{Error{ECate::eTCap, ECode::eUnexValue, "invalid unicode"}};
        }
    }

    return utf8Str;
}

}  // namespace tcap::_i
