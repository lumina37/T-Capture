#pragma once

#include <expected>

#include "tcap/helper/error.hpp"

namespace tcap::mf {

class DeviceBox {
    DeviceBox(int fd) noexcept;

public:
    DeviceBox& operator=(const DeviceBox&) = delete;
    DeviceBox(const DeviceBox&) = delete;
    DeviceBox(DeviceBox&& rhs) noexcept;
    DeviceBox& operator=(DeviceBox&& rhs) noexcept;
    ~DeviceBox() noexcept;

    [[nodiscard]] static std::expected<DeviceBox, Error> create(int fd) noexcept;

    [[nodiscard]] int getFd() const noexcept { return fd_; }

private:
    int fd_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/device/box.cpp"
#endif
