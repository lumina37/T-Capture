#include <expected>

#include <pipewire/pipewire.h>

#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/utils/pw/call_once.hpp"
#endif

namespace tcap::pw {

std::expected<void, Error> globalInit() noexcept {
    pw_init(nullptr, nullptr);

    return {};
}

void globalDestroy() noexcept {}

}  // namespace tcap::pw
