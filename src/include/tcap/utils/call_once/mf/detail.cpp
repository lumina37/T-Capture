#include <expected>

#include <mfapi.h>
#include <mfidl.h>

#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/utils/call_once/mf/detail.hpp"
#endif

namespace tcap::mf {

std::expected<void, Error> initCOM() noexcept {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        return std::unexpected{Error{ECate::eMF, hr}};
    }
    return {};
}

std::expected<void, Error> initMF() noexcept {
    HRESULT hr = MFStartup(MF_VERSION);
    if (FAILED(hr)) {
        return std::unexpected{Error{ECate::eMF, hr}};
    }
    return {};
}

std::expected<void, Error> globalInit() noexcept {
    auto comRes = initCOM();
    if (!comRes) return comRes;

    auto mfRes = initMF();
    if (!mfRes) return mfRes;

    return {};
}

void globalDestroy() noexcept {
    // Must keep the following order
    MFShutdown();
    CoUninitialize();
}

}  // namespace tcap::mf
