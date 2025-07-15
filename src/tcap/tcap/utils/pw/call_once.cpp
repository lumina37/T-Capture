#include <expected>

#include <pipewire/pipewire.h>

#include "tcap/utils/error.hpp"
#include "tcap/utils/pw/call_once.hpp"

namespace tcap::pw {

std::expected<void, Error> globalInit() noexcept {
    pw_init(nullptr, nullptr);
    return {};
}

void globalDestroy() noexcept {}

}  // namespace tcap::pw
