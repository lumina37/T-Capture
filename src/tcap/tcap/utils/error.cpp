#include <source_location>
#include <string>
#include <utility>

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/utils/error.hpp"
#endif

namespace tcap {

Error::Error() noexcept : cate(ECate::eSuccess), code(0) {}

Error::Error(const ECate cate, const int code, const std::source_location& source) noexcept
    : cate(cate), code(code), source(source) {}

Error::Error(const ECate cate, const int code, std::string&& msg, const std::source_location& source) noexcept
    : cate(cate), code(code), source(source), msg(std::move(msg)) {}

Error::Error(const ECate cate, const ECode code, const std::source_location& source) noexcept
    : cate(cate), code((int)code), source(source) {}

Error::Error(const ECate cate, const ECode code, std::string&& msg, const std::source_location& source) noexcept
    : cate(cate), code((int)code), source(source), msg(std::move(msg)) {}

}  // namespace tcap
