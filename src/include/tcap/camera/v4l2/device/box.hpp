#pragma once

#include <expected>
#include <filesystem>

#include "tcap/helper/error.hpp"

namespace tcap::v4l2 {

namespace fs = std::filesystem;

class DeviceBox {
    DeviceBox(int fd) noexcept;

public:
    DeviceBox& operator=(const DeviceBox&) = delete;
    DeviceBox(const DeviceBox&) = delete;
    DeviceBox(DeviceBox&& rhs) noexcept;
    DeviceBox& operator=(DeviceBox&& rhs) noexcept;
    ~DeviceBox() noexcept;

    [[nodiscard]] static std::expected<DeviceBox, Error> create(const fs::path& path) noexcept;

    [[nodiscard]] int getFd() const noexcept { return fd_; }

private:
    int fd_;
};

}  // namespace tcap::v4l2

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/device/box.cpp"
#endif
