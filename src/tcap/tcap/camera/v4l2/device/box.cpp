#include <expected>
#include <format>
#include <utility>

#include <fcntl.h>
#include <unistd.h>

#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/device/box.hpp"
#endif

namespace tcap::v4l2 {

DeviceBox::DeviceBox(const int fd) noexcept : fd_(fd) {}

DeviceBox::DeviceBox(DeviceBox&& rhs) noexcept : fd_(std::exchange(rhs.fd_, -1)) {}

DeviceBox& DeviceBox::operator=(DeviceBox&& rhs) noexcept {
    fd_ = std::exchange(rhs.fd_, -1);
    return *this;
}

DeviceBox::~DeviceBox() noexcept {
    if (fd_ < 0) return;
    close(fd_);
    fd_ = -1;
}

std::expected<DeviceBox, Error> DeviceBox::create(const fs::path& path) noexcept {
    const int fd = open(path.c_str(), O_RDWR | O_NONBLOCK);
    if (fd < 0) {
        auto errMsg = std::format("failed to open: {}", path.c_str());
        return std::unexpected{Error{ECate::eV4L2, errno, std::move(errMsg)}};
    }

    return DeviceBox{fd};
}

}  // namespace tcap::v4l2
