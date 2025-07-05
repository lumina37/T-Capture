#pragma once

#include <expected>
#include <string>

#include <dbus/dbus.h>

#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::dbus {

class ErrorBox {
public:
    ErrorBox() noexcept;
    ErrorBox& operator=(const ErrorBox&) = delete;
    ErrorBox(const ErrorBox&) = delete;
    TCAP_API ErrorBox(ErrorBox&& rhs) noexcept;
    TCAP_API ErrorBox& operator=(ErrorBox&& rhs) noexcept;
    TCAP_API ~ErrorBox() noexcept;

    [[nodiscard]] TCAP_API DBusError* getPError() noexcept { return &error_; }
    [[nodiscard]] TCAP_API DBusError& getError() noexcept { return error_; }
    [[nodiscard]] TCAP_API const DBusError& getError() const noexcept { return error_; }
    [[nodiscard]] TCAP_API std::string_view getName() const noexcept { return error_.name; }
    [[nodiscard]] TCAP_API std::string_view getMessage() const noexcept { return error_.message; }
    [[nodiscard]] TCAP_API bool hasError() const noexcept { return dbus_error_is_set(&error_); }
    [[nodiscard]] TCAP_API Error toError() const noexcept { return {ECate::eDBus, 0, error_.message}; }

private:
    DBusError error_;
};

}  // namespace tcap::dbus

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/dbus/error.cpp"
#endif
