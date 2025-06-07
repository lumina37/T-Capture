#pragma once

#include <expected>

#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class SampleBox {
    SampleBox(IMFSample* pSample) noexcept;

public:
    SampleBox(SampleBox&& rhs) noexcept;
    ~SampleBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<SampleBox, Error> create(IMFSample* pSample) noexcept;

    [[nodiscard]] TCAP_API IMFSample* getPSample() const noexcept { return pSample_; }

private:
    IMFSample* pSample_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/sample.cpp"
#endif
