#pragma once

#include <expected>

#include <dbus/dbus.h>

#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::dbus {

class ReqCreateSessionBox {
    ReqCreateSessionBox(DBusMessage* pMessage) noexcept;

public:
    ReqCreateSessionBox& operator=(const ReqCreateSessionBox&) = delete;
    ReqCreateSessionBox(const ReqCreateSessionBox&) = delete;
    TCAP_API ReqCreateSessionBox(ReqCreateSessionBox&& rhs) noexcept;
    TCAP_API ReqCreateSessionBox& operator=(ReqCreateSessionBox&& rhs) noexcept;
    TCAP_API ~ReqCreateSessionBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<ReqCreateSessionBox, Error> create(
        std::string_view sessionHandleToken) noexcept;

    [[nodiscard]] TCAP_API DBusMessage* getPMessage() noexcept { return pMessage_; }

private:
    DBusMessage* pMessage_;
};

}  // namespace tcap::dbus
