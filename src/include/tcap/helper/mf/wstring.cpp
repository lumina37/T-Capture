#include <utility>

#include <mfobjects.h>

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/helper/mf/wstring.hpp"
#endif

namespace tcap::mf {

WStringBox::WStringBox(WCHAR* pWString, UINT32 len) noexcept : pWString_(pWString), len_(len) {}

WStringBox::WStringBox(WStringBox&& rhs) noexcept : pWString_(std::exchange(rhs.pWString_, nullptr)), len_(rhs.len_) {}

WStringBox& WStringBox::operator=(WStringBox&& rhs) noexcept {
    pWString_ = std::exchange(rhs.pWString_, nullptr);
    len_ = rhs.len_;
    return *this;
}

WStringBox::~WStringBox() noexcept {
    if (pWString_ == nullptr) return;
    CoTaskMemFree(pWString_);
    pWString_ = nullptr;
}

}  // namespace tcap::mf
