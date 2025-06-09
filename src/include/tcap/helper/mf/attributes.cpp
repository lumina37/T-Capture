#include <expected>

#include <atlbase.h>
#include <mfapi.h>
#include <mfobjects.h>

#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/helper/mf/attributes.hpp"
#endif

namespace tcap::mf {

AttributesBox::AttributesBox(CComPtr<IMFAttributes>&& pAttributes) noexcept : pAttributes_(std::move(pAttributes)) {}

std::expected<AttributesBox, Error> AttributesBox::create(int size) noexcept {
    IMFAttributes* pAttributes;
    HRESULT hr = MFCreateAttributes(&pAttributes, size);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "MFCreateAttributes failed"}};
    }

    return AttributesBox{pAttributes};
}

std::expected<void, Error> AttributesBox::setGuid(const GUID& key, const GUID& value) noexcept {
    HRESULT hr = pAttributes_->SetGUID(key, value);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pAttributes_->SetGUID failed"}};
    }
    return {};
}

std::expected<void, Error> AttributesBox::setUnknown(const GUID& key, IUnknown* pUnknown) noexcept {
    HRESULT hr = pAttributes_->SetUnknown(key, pUnknown);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pAttributes_->SetUnknown failed"}};
    }
    return {};
}

}  // namespace tcap::mf
