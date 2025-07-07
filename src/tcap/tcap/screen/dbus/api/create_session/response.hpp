#pragma once

#include <expected>

#include <dbus/dbus.h>

#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::dbus {

class ResCreateSessionBox {
    ResCreateSessionBox(DBusMessage* pMessage, std::string_view sessionHandle) noexcept;

public:
    ResCreateSessionBox& operator=(const ResCreateSessionBox&) = delete;
    ResCreateSessionBox(const ResCreateSessionBox&) = delete;
    TCAP_API ResCreateSessionBox(ResCreateSessionBox&& rhs) noexcept;
    TCAP_API ResCreateSessionBox& operator=(ResCreateSessionBox&& rhs) noexcept;
    TCAP_API ~ResCreateSessionBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<ResCreateSessionBox, Error> create(DBusMessage* pMessage) noexcept;

    [[nodiscard]] TCAP_API DBusMessage* getPMessage() noexcept { return pMessage_; }
    [[nodiscard]] TCAP_API std::string_view getSessionHandle() noexcept { return sessionHandle_; }

private:
    DBusMessage* pMessage_;
    std::string_view sessionHandle_;
};

}  // namespace tcap::dbus

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/dbus/message/create_session/response.cpp"
#endif
