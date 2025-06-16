#pragma once

#include <cstdint>

namespace tcap::v4l2 {

class Fps {
public:
    uint32_t numerator_;
    uint32_t denominator_;
};

}  // namespace tcap::v4l2
