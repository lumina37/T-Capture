#pragma once

#include <expected>

#include <atlbase.h>
#include <mfobjects.h>

#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class SampleBox {
    SampleBox(CComPtr<IMFSample>&& pSample) noexcept;

public:
    SampleBox(const SampleBox&) = delete;
    TCAP_API SampleBox(SampleBox&&) noexcept = default;

    [[nodiscard]] TCAP_API static std::expected<SampleBox, Error> create(CComPtr<IMFSample>&& pSample) noexcept;

    [[nodiscard]] TCAP_API IMFSample* getPSample() const noexcept { return pSample_; }

private:
    CComPtr<IMFSample> pSample_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/sample.cpp"
#endif
