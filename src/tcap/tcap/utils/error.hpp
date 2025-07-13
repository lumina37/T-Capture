#pragma once

#include <source_location>
#include <string>
#include <utility>

namespace tcap {

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
    eTCap,
    eSys,
    eMF,
    eV4L2,
    eX11,
    eDBus,
    ePW,
};

constexpr std::string_view errCateToStr(const ECate cate) noexcept {
    switch (cate) {
        case ECate::eSuccess:
            return "Success";
        case ECate::eUnknown:
            return "Unknown";
        case ECate::eMisc:
            return "Misc";
        case ECate::eTCap:
            return "TCap";
        case ECate::eSys:
            return "System";
        case ECate::eMF:
            return "MF";
        case ECate::eV4L2:
            return "V4L2";
        case ECate::eDBus:
            return "DBus";
        case ECate::ePW:
            return "PW";
        case ECate::eX11:
            return "X11";
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

    Error() noexcept : cate(ECate::eSuccess), code(0) {}

    template <typename T>
    Error(const ECate cate, T code, const std::source_location& source = std::source_location::current()) noexcept
        : cate(cate), code((int)code), source(source) {}

    template <typename T>
    Error(const ECate cate, T code, std::string&& msg,
          const std::source_location& source = std::source_location::current()) noexcept
        : cate(cate), code((int)code), source(source), msg(std::move(msg)) {}

    Error& operator=(const Error& rhs) = default;
    Error(const Error& rhs) = default;
    Error& operator=(Error&& rhs) = default;
    Error(Error&& rhs) noexcept = default;
};

}  // namespace tcap
