#include <expected>

#include <mfapi.h>
#include <mfobjects.h>

#include "tcap/helper/mf/attributes.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::mf::_i {

AttributesBox::AttributesBox(IMFAttributes* pAttributes) noexcept : pAttributes_(pAttributes) {}

AttributesBox::AttributesBox(AttributesBox&& rhs) noexcept : pAttributes_(std::exchange(rhs.pAttributes_, nullptr)) {}

AttributesBox& AttributesBox::operator=(AttributesBox&& rhs) noexcept {
    pAttributes_ = std::exchange(rhs.pAttributes_, nullptr);
    return *this;
}

AttributesBox::~AttributesBox() noexcept {
    if (pAttributes_ == nullptr) return;
    pAttributes_->Release();
    pAttributes_ = nullptr;
}

std::expected<AttributesBox, Error> AttributesBox::create(const int size) noexcept {
    IMFAttributes* pAttributes;
    const HRESULT hr = MFCreateAttributes(&pAttributes, size);
    if (FAILED(hr)) {
        return std::unexpected{Error{ECate::eMF, hr}};
    }

    return AttributesBox{pAttributes};
}

std::expected<void, Error> AttributesBox::setGuid(const GUID& key, const GUID& value) noexcept {
    HRESULT hr = pAttributes_->SetGUID(key, value);
    if (FAILED(hr)) {
        return std::unexpected{Error{ECate::eMF, hr}};
    }
    return {};
}

std::expected<void, Error> AttributesBox::setUnknown(const GUID& key, IUnknown* pUnknown) noexcept {
    HRESULT hr = pAttributes_->SetUnknown(key, pUnknown);
    if (FAILED(hr)) {
        return std::unexpected{Error{ECate::eMF, hr}};
    }
    return {};
}

}  // namespace tcap::mf::_i
