#pragma once

#include <expected>

#include <mfobjects.h>

#include "tcap/helper/error.hpp"

namespace tcap::mf {

class AttributesBox {
    AttributesBox(IMFAttributes* pAttributes) noexcept;

public:
    AttributesBox(AttributesBox&& rhs) noexcept;
    ~AttributesBox() noexcept;

    [[nodiscard]] static std::expected<AttributesBox, Error> create(int size) noexcept;

    [[nodiscard]] IMFAttributes* getPAttributes() const noexcept { return pAttributes_; }

    [[nodiscard]] std::expected<void, Error> setGuid(const GUID& key, const GUID& value) noexcept;
    [[nodiscard]] std::expected<void, Error> setUnknown(const GUID& key, IUnknown* pUnknown) noexcept;

private:
    IMFAttributes* pAttributes_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/helper/mf/arrtibutes.cpp"
#endif
