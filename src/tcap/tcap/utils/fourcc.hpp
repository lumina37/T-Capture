#pragma once

#include <array>
#include <cstdint>
#include <string>

#include "tcap/common/defines.h"

namespace tcap {

class FourCC {
public:
    static constexpr int SIZE = 4;

    TCAP_API constexpr FourCC(const uint32_t format) noexcept {
        for (int i = 0; i < SIZE; ++i) {
            arr_[i] = (char)(format >> (i * 8) & 0xFF);
        }
        arr_[SIZE] = 0;
    }

    [[nodiscard]] TCAP_API constexpr std::string_view strView() const noexcept { return {arr_.data(), SIZE}; }
    [[nodiscard]] TCAP_API constexpr const char* cStr() const noexcept { return arr_.data(); }

private:
    std::array<char, SIZE + 1> arr_;
};

}  // namespace tcap
