#pragma once

#include <source_location>
#include <string>

namespace tlp {

enum class ECode {
    eUnknown = 0,
    eUnexValue = 1,        // Unexpected Value
    eNoSupport = 2,        // Feature Not Supported
    eNoImpl = 3,           // Not Implemented
    eResourceInvalid = 3,  // Resource is Invalid
};

enum class ECate {
    eSuccess = 0,
    eUnknown,
    eMisc,
    eTLoop,
    eSys,
    eIOCP,
};

constexpr std::string_view errCateToStr(const ECate cate) noexcept {
    switch (cate) {
        case ECate::eSuccess:
            return "Success";
        case ECate::eUnknown:
            return "Unknown";
        case ECate::eMisc:
            return "Misc";
        case ECate::eTLoop:
            return "TLoop";
        case ECate::eSys:
            return "System";
        case ECate::eIOCP:
            return "IOCP";
        default:
            return "Unknown";
    }
}

class Error {
public:
    ECate cate;
    int code;
    std::source_location source;
    std::string msg;

    Error() noexcept;
    Error(ECate cate, int code, const std::source_location& source = std::source_location::current()) noexcept;
    Error(ECate cate, int code, std::string&& msg,
          const std::source_location& source = std::source_location::current()) noexcept;
    Error(ECate cate, ECode code, const std::source_location& source = std::source_location::current()) noexcept;
    Error(ECate cate, ECode code, std::string&& msg,
          const std::source_location& source = std::source_location::current()) noexcept;
    Error& operator=(const Error& rhs) = default;
    Error(const Error& rhs) = default;
    Error& operator=(Error&& rhs) = default;
    Error(Error&& rhs) noexcept = default;
};

}  // namespace tlp

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tloop/utils/error.cpp"
#endif
