#pragma once

#include <cstdint>
#include <string>

namespace tcap {

class FourCC {
public:
    static constexpr int SIZE = 4;

    constexpr FourCC(uint32_t format) {
        for (int i = 0; i < SIZE; ++i) {
            arr_[i] = (char)(format >> (i * 8) & 0xFF);
        }
        arr_[SIZE] = 0;
    }

    [[nodiscard]] constexpr std::string_view strView() const noexcept { return {arr_.data(), SIZE}; }
    [[nodiscard]] constexpr const char* cStr() const noexcept { return arr_.data(); }

private:
    std::array<char, SIZE + 1> arr_;
};

}  // namespace tcap
