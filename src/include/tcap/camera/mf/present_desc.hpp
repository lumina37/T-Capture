#pragma once

#include <expected>
#include <vector>

#include <mfidl.h>

#include "tcap/camera/mf/source.hpp"
#include "tcap/camera/mf/stream_desc.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class PresentDescBox {
    PresentDescBox(IMFPresentationDescriptor* pPresentDesc, std::vector<StreamDescBox>&& streamDescBoxes) noexcept;

public:
    PresentDescBox(PresentDescBox&& rhs) noexcept;
    ~PresentDescBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<PresentDescBox, Error> create(const SourceBox& sourceBox) noexcept;

    [[nodiscard]] TCAP_API IMFPresentationDescriptor* getPPresentDesc() const noexcept { return pPresentDesc_; }
    [[nodiscard]] TCAP_API StreamDescBox& getStreamDescBox(int index) noexcept { return streamDescBoxes_[index]; }
    [[nodiscard]] TCAP_API const StreamDescBox& getStreamDescBox(int index) const noexcept {
        return streamDescBoxes_[index];
    }
    [[nodiscard]] TCAP_API const std::vector<StreamDescBox>& getStreamDescBoxes() const noexcept {
        return streamDescBoxes_;
    }
    [[nodiscard]] TCAP_API std::vector<StreamDescBox>& getStreamDescBoxes() noexcept { return streamDescBoxes_; }

private:
    IMFPresentationDescriptor* pPresentDesc_;
    std::vector<StreamDescBox> streamDescBoxes_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/present_desc.cpp"
#endif
