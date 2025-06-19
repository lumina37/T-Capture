#include <format>
#include <utility>

#include <glob.h>

#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/v4l2/device/path.hpp"
#endif

namespace tcap::v4l2 {

namespace fs = std::filesystem;

int DevicePaths::onError(const char* failedPath, int errNo) noexcept {
    auto errMsg = std::format("failed on path: {}", failedPath);
    createError = {ECate::eSys, errNo, std::move(errMsg)};
    return 1;
}

DevicePaths::DevicePaths(std::vector<fs::path>&& paths) noexcept : paths_(std::move(paths)) {}

thread_local Error DevicePaths::createError{};

std::expected<DevicePaths, Error> DevicePaths::create() noexcept {
    glob_t globRes;
    int errNo = glob("/dev/video*", GLOB_NOSORT, onError, &globRes);
    if (createError.code != 0) {
        return std::unexpected{std::exchange(createError, Error{ECate::eUnknown, 0})};
    }
    if (errNo != 0) {
        return std::unexpected{Error{ECate::eSys, errNo}};
    }

    std::vector<fs::path> devicePaths;
    devicePaths.reserve(globRes.gl_pathc);
    for (size_t i = 0; i < globRes.gl_pathc; ++i) {
        devicePaths.emplace_back(globRes.gl_pathv[i]);
    }

    globfree(&globRes);

    return DevicePaths{std::move(devicePaths)};
}

}  // namespace tcap::v4l2
