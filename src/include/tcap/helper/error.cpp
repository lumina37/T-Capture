#include <source_location>
#include <string>
#include <utility>

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/helper/error.hpp"
#endif

namespace tcap {

Error::Error(const int code, const std::source_location& source) : code(code), source(source) {}

Error::Error(const int code, const std::string& msg, const std::source_location& source)
    : code(code), source(source), msg(msg) {}

Error::Error(const int code, std::string&& msg, const std::source_location& source)
    : code(code), source(source), msg(std::move(msg)) {}

}  // namespace tcap
