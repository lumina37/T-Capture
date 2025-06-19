#pragma once

#include <source_location>
#include <string>

namespace tcap {

class Error {
public:
    int code;
    std::source_location source;
    std::string msg;

    Error() = default;
    explicit Error(int code, const std::source_location& source = std::source_location::current());
    Error(int code, const std::string& msg, const std::source_location& source = std::source_location::current());
    Error(int code, std::string&& msg, const std::source_location& source = std::source_location::current());
    Error& operator=(const Error& rhs) = default;
    Error(const Error& rhs) = default;
    Error& operator=(Error&& rhs) = default;
    Error(Error&& rhs) noexcept = default;
};

}  // namespace tcap

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/utils/error.cpp"
#endif
