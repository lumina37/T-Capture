#include <mfidl.h>

#include "tcap/camera/mf/source.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/present_desc.hpp"
#endif

namespace tcap::mf {

PresentDescBox::PresentDescBox(IMFPresentationDescriptor* pPresentDesc) noexcept : pPresentDesc_(pPresentDesc) {}

PresentDescBox::PresentDescBox(PresentDescBox&& rhs) noexcept
    : pPresentDesc_(std::exchange(rhs.pPresentDesc_, nullptr)) {}

PresentDescBox::~PresentDescBox() noexcept {
    if (pPresentDesc_ == nullptr) return;
    pPresentDesc_->Release();
    pPresentDesc_ = nullptr;
}

std::expected<PresentDescBox, Error> PresentDescBox::create(const SourceBox& sourceBox) noexcept {
    IMFPresentationDescriptor* pPresentDesc;

    auto pSource = sourceBox.getPSource();
    HRESULT hr = pSource->CreatePresentationDescriptor(&pPresentDesc);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pSource->CreatePresentationDescriptor failed"}};
    }

    return PresentDescBox{pPresentDesc};
}

}  // namespace tcap::mf
