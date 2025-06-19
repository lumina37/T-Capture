#pragma once

#include <string>

#include <mfobjects.h>

#include "tcap/common/defines.h"

namespace tcap::mf {

class WStringBox {
public:
    TCAP_API WStringBox(WCHAR* pWString, UINT32 len) noexcept;

    TCAP_API WStringBox(WStringBox&& rhs) noexcept;
    TCAP_API WStringBox& operator=(WStringBox&& rhs) noexcept;
    TCAP_API ~WStringBox() noexcept;

    [[nodiscard]] TCAP_API WCHAR* getPWString() const noexcept { return pWString_; }
    [[nodiscard]] TCAP_API std::wstring_view getWStringView() const noexcept { return {pWString_, len_}; }

private:
    WCHAR* pWString_;
    UINT32 len_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/utils/wstring.cpp"
#endif
