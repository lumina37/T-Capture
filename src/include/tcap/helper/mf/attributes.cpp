#include <expected>

#include <mfapi.h>
#include <mfobjects.h>

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/helper/mf/attributes.hpp"
#endif

namespace tcap::mf {

AttributesBox::AttributesBox(IMFAttributes* pAttributes) noexcept : pAttributes_(pAttributes) {}

AttributesBox::AttributesBox(AttributesBox&& rhs) noexcept : pAttributes_(std::exchange(rhs.pAttributes_, nullptr)) {}

AttributesBox::~AttributesBox() noexcept {
    if (pAttributes_ == nullptr) return;
    pAttributes_->Release();
    pAttributes_ = nullptr;
}

std::expected<AttributesBox, Error> AttributesBox::create(int size) noexcept {
    IMFAttributes* pAttributes;
    HRESULT hr = MFCreateAttributes(&pAttributes, size);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "MFCreateAttributes failed"}};
    }

    return AttributesBox{pAttributes};
}

std::expected<void, Error> AttributesBox::set(const IID& key, const IID& value) noexcept {
    HRESULT hr = pAttributes_->SetGUID(key, value);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pAttributes_->SetGUID failed"}};
    }
    return {};
}

}  // namespace tcap::mf
