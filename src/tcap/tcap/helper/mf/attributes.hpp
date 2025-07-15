#pragma once

#include <expected>

#include <mfobjects.h>

#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::mf::_i {

class AttributesBox {
    AttributesBox(IMFAttributes* pAttributes) noexcept;

public:
    AttributesBox(const AttributesBox&) = delete;
    AttributesBox& operator=(const AttributesBox&) = delete;
    TCAP_API AttributesBox(AttributesBox&& rhs) noexcept;
    TCAP_API AttributesBox& operator=(AttributesBox&& rhs) noexcept;
    TCAP_API ~AttributesBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<AttributesBox, Error> create(int size) noexcept;

    [[nodiscard]] TCAP_API IMFAttributes* getPAttributes() const noexcept { return pAttributes_; }

    [[nodiscard]] TCAP_API std::expected<void, Error> setGuid(const GUID& key, const GUID& value) noexcept;
    [[nodiscard]] TCAP_API std::expected<void, Error> setUnknown(const GUID& key, IUnknown* pUnknown) noexcept;

private:
    IMFAttributes* pAttributes_;
};

}  // namespace tcap::mf::_i
