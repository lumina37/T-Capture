#include <format>
#include <memory>

#include <glob.h>

#include <utility>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/device/boxes.hpp"
#endif

namespace tcap::v4l2 {

int DeviceBoxes::onError(const char* failedPath, int errNo) noexcept {
    auto errMsg = std::format("failed on path: {}", failedPath);
    createError = {errNo, errMsg};
    return 1;
}

DeviceBoxes::DeviceBoxes(std::vector<std::shared_ptr<DeviceBox>>&& deviceBoxes) noexcept
    : pDeviceBoxes_(std::move(deviceBoxes)) {}

thread_local Error DeviceBoxes::createError{};

std::expected<DeviceBoxes, Error> DeviceBoxes::create() noexcept {
    glob_t globRes;
    int errNo = glob("/dev/video*", GLOB_NOSORT, onError, &globRes);
    if (createError.code != 0) {
        return std::unexpected{std::exchange(createError, {})};
    }
    if (errNo != 0) {
        return std::unexpected{Error{errNo, "failed to glob devices"}};
    }

    std::vector<std::shared_ptr<DeviceBox>> deviceBoxes;
    deviceBoxes.reserve(globRes.gl_pathc);
    for (size_t i = 0; i < globRes.gl_pathc; ++i) {
        auto deviceBoxRes = DeviceBox::create(globRes.gl_pathv[i]);
        if (!deviceBoxRes) return std::unexpected{std::move(deviceBoxRes.error())};
        auto pDeviceBox = std::make_shared<DeviceBox>(std::move(deviceBoxRes.value()));
        deviceBoxes.push_back(std::move(pDeviceBox));
    }

    globfree(&globRes);

    return DeviceBoxes{std::move(deviceBoxes)};
}

}  // namespace tcap::v4l2
