#pragma once

#include <expected>

#include <mfobjects.h>

#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class AttributesBox {
    AttributesBox(IMFAttributes* pAttributes) noexcept;

public:
    TCAP_API AttributesBox(AttributesBox&& rhs) noexcept;
    TCAP_API ~AttributesBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<AttributesBox, Error> create(int size) noexcept;

    [[nodiscard]] TCAP_API IMFAttributes* getPAttributes() const noexcept { return pAttributes_; }

    [[nodiscard]] TCAP_API std::expected<void, Error> setGuid(const GUID& key, const GUID& value) noexcept;
    [[nodiscard]] TCAP_API std::expected<void, Error> setUnknown(const GUID& key, IUnknown* pUnknown) noexcept;

private:
    IMFAttributes* pAttributes_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/helper/mf/arrtibutes.cpp"
#endif
