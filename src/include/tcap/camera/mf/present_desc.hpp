#pragma once

#include <expected>

#include <mfidl.h>

#include "tcap/camera/mf/source.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class PresentDescBox {
    PresentDescBox(IMFPresentationDescriptor* pPresentDesc) noexcept;

public:
    PresentDescBox(PresentDescBox&& rhs) noexcept;
    ~PresentDescBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<PresentDescBox, Error> create(const SourceBox& sourceBox) noexcept;

    [[nodiscard]] TCAP_API IMFPresentationDescriptor* getPPresentDesc() const noexcept { return pPresentDesc_; }

private:
    IMFPresentationDescriptor* pPresentDesc_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/present_desc.cpp"
#endif
