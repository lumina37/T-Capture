#pragma once

#include <string>

#include <mfobjects.h>

namespace tcap::mf {

class WStringBox {
public:
    WStringBox(WCHAR* pWString, UINT32 len) noexcept;

    WStringBox(WStringBox&& rhs) noexcept;
    ~WStringBox() noexcept;

    [[nodiscard]] WCHAR* getPWString() const noexcept { return pWString_; }
    [[nodiscard]] std::wstring_view getWStringView() const noexcept { return {pWString_, len_}; }

private:
    WCHAR* pWString_;
    UINT32 len_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/helper/mf/wstring.cpp"
#endif
