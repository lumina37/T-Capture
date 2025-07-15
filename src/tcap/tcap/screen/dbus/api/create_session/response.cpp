#include <utility>

#include <dbus/dbus.h>

#include "tcap/screen/dbus/api/create_session/response.hpp"
#include "tcap/screen/dbus/arg.hpp"
#include "tcap/utils/error.hpp"

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
    _i::MsgArgRootGetter argRoot{pMessage};
    const _i::MsgArgStrGetter argString{argRoot};
    return ResCreateSessionBox{pMessage, argString.get()};
}

}  // namespace tcap::dbus
