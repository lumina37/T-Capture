#include <unistd.h>

#include <expected>
#include <utility>

#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/device/box.hpp"
#endif

namespace tcap::mf {

DeviceBox::DeviceBox(int fd) noexcept : fd_(fd) {}

DeviceBox::DeviceBox(DeviceBox&& rhs) noexcept : fd_(std::exchange(rhs.fd_, nullptr)) {}

DeviceBox& DeviceBox::operator=(DeviceBox&& rhs) noexcept {
    fd_ = std::exchange(rhs.fd_, 0);
    return *this;
}

DeviceBox::~DeviceBox() noexcept {
    if (fd_ == 0) return;
    close(fd_);
    fd_ = 0;
}

std::expected<DeviceBox, Error> DeviceBox::create(int fd) noexcept { return DeviceBox{fd}; }

}  // namespace tcap::mf
