#pragma once

#include <vector>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::v4l2 {

class FpsBox {
public:
    TCAP_API constexpr FpsBox(uint32_t numerator, uint32_t denominator) noexcept;

    [[nodiscard]] TCAP_API static std::expected<std::vector<FpsBox>, Error> createBoxes(const DeviceBox& deviceBox,
                                                                                        uint32_t format, uint32_t width,
                                                                                        uint32_t height) noexcept;

    [[nodiscard]] TCAP_API constexpr uint32_t getNumerator() const noexcept { return numerator_; }
    [[nodiscard]] TCAP_API constexpr uint32_t getDenominator() const noexcept { return denominator_; }
    [[nodiscard]] TCAP_API constexpr float approxFps() const noexcept { return (float)denominator_ / (float)numerator_; }

    TCAP_API constexpr void setNumerator(const uint32_t numerator) noexcept { numerator_ = numerator; }
    TCAP_API constexpr void setDenominator(const uint32_t denominator) noexcept { denominator_ = denominator; }

    [[nodiscard]] TCAP_API std::expected<void, Error> apply(DeviceBox& deviceBox) const noexcept;

private:
    uint32_t numerator_;
    uint32_t denominator_;
};

}  // namespace tcap::v4l2
