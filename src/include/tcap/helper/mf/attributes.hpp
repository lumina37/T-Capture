#pragma once

#include <expected>

#include <atlbase.h>
#include <mfobjects.h>

#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class AttributesBox {
    AttributesBox(CComPtr<IMFAttributes>&& pAttributes) noexcept;

public:
    AttributesBox(const AttributesBox&) = delete;
    TCAP_API AttributesBox(AttributesBox&& rhs) noexcept = default;

    [[nodiscard]] TCAP_API static std::expected<AttributesBox, Error> create(int size) noexcept;

    [[nodiscard]] TCAP_API IMFAttributes* getPAttributes() const noexcept { return pAttributes_; }

    [[nodiscard]] TCAP_API std::expected<void, Error> setGuid(const GUID& key, const GUID& value) noexcept;
    [[nodiscard]] TCAP_API std::expected<void, Error> setUnknown(const GUID& key, IUnknown* pUnknown) noexcept;

private:
    CComPtr<IMFAttributes> pAttributes_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/helper/mf/arrtibutes.cpp"
#endif
