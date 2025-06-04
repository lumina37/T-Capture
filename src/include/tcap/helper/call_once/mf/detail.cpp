#include <expected>
#include <format>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <mfapi.h>
#include <mfidl.h>

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/helper/call_once/mf/detail.hpp"
#endif

namespace tcap::mf {

std::expected<void, Error> initCOM() noexcept {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        auto errMsg = std::format("CoInitializeEx failed");
        return std::unexpected{Error{hr, std::move(errMsg)}};
    }
    return {};
}

std::expected<void, Error> initMF() noexcept {
    HRESULT hr = MFStartup(MF_VERSION);
    if (FAILED(hr)) {
        auto errMsg = std::format("MFStartup failed");
        return std::unexpected{Error{hr, std::move(errMsg)}};
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
