#include <mfidl.h>

#include "tcap/camera/mf/source.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/present_desc.hpp"
#endif

namespace tcap::mf {

PresentDescBox::PresentDescBox(IMFPresentationDescriptor* pPresentDesc,
                               std::vector<StreamDescBox>&& streamDescBoxes) noexcept
    : pPresentDesc_(pPresentDesc), streamDescBoxes_(std::move(streamDescBoxes)) {}

PresentDescBox::PresentDescBox(PresentDescBox&& rhs) noexcept
    : pPresentDesc_(std::exchange(rhs.pPresentDesc_, nullptr)), streamDescBoxes_(std::move(rhs.streamDescBoxes_)) {}

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

    DWORD streamDescCount;
    pPresentDesc->GetStreamDescriptorCount(&streamDescCount);

    std::vector<StreamDescBox> streamDescBoxes;
    streamDescBoxes.reserve(streamDescCount);
    for (DWORD i = 0; i < streamDescCount; i++) {
        IMFStreamDescriptor* pStreamDesc;
        [[maybe_unused]] BOOL selected;
        hr = pPresentDesc->GetStreamDescriptorByIndex(i, &selected, &pStreamDesc);
        if (FAILED(hr)) {
            return std::unexpected{Error{hr, "pPresentDesc->GetStreamDescriptorByIndex failed"}};
        }

        auto streamDescBoxRes = StreamDescBox::create(pStreamDesc);
        if (!streamDescBoxRes) return std::unexpected{std::move(streamDescBoxRes.error())};
        streamDescBoxes.push_back(std::move(streamDescBoxRes.value()));
    }

    return PresentDescBox{pPresentDesc, std::move(streamDescBoxes)};
}

}  // namespace tcap::mf
