#include <utility>

#include <dbus/dbus.h>

#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/dbus/message/create_session/response.hpp"
#endif

namespace tcap::dbus {

ResCreateSessionBox::ResCreateSessionBox(DBusMessage* pMessage, std::string_view sessionHandle) noexcept
    : pMessage_(pMessage), sessionHandle_(sessionHandle) {}

ResCreateSessionBox::ResCreateSessionBox(ResCreateSessionBox&& rhs) noexcept
    : pMessage_(std::exchange(rhs.pMessage_, nullptr)), sessionHandle_(rhs.sessionHandle_) {}

ResCreateSessionBox& ResCreateSessionBox::operator=(ResCreateSessionBox&& rhs) noexcept {
    pMessage_ = std::exchange(rhs.pMessage_, nullptr);
    sessionHandle_ = rhs.sessionHandle_;
    return *this;
}

ResCreateSessionBox::~ResCreateSessionBox() noexcept {
    if (pMessage_ == nullptr) return;
    dbus_message_unref(pMessage_);
    pMessage_ = nullptr;
}

std::expected<ResCreateSessionBox, Error> ResCreateSessionBox::create(DBusMessage* pMessage) noexcept {
    DBusMessageIter itRoot;
    dbus_message_iter_init(pMessage, &itRoot);

    const char* pSessionHandle;
    dbus_message_iter_get_basic(&itRoot, &pSessionHandle);
    std::string_view sessionHandle{pSessionHandle};

    return ResCreateSessionBox{pMessage, sessionHandle};
}

}  // namespace tcap::dbus
