#pragma once

#include <expected>
#include <filesystem>
#include <vector>

#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::v4l2 {

namespace fs = std::filesystem;

class DevicePaths {
    DevicePaths(std::vector<fs::path>&& paths) noexcept;

    static thread_local Error createError;
    static int onError(const char* failedPath, int errNo) noexcept;

public:
    DevicePaths(const DevicePaths&) = delete;
    DevicePaths& operator=(const DevicePaths&) = delete;
    TCAP_API DevicePaths(DevicePaths&&) noexcept = default;
    TCAP_API DevicePaths& operator=(DevicePaths&&) noexcept = default;

    [[nodiscard]] TCAP_API static std::expected<DevicePaths, Error> create() noexcept;

    [[nodiscard]] TCAP_API int size() const noexcept { return (int)paths_.size(); }
    [[nodiscard]] TCAP_API bool empty() const noexcept { return paths_.empty(); }
    [[nodiscard]] TCAP_API const fs::path& getPath(const int index) const noexcept { return paths_[index]; }
    [[nodiscard]] TCAP_API const std::vector<fs::path>& getPaths() const noexcept { return paths_; }
    [[nodiscard]] TCAP_API std::vector<fs::path>& getPaths() noexcept { return paths_; }

private:
    std::vector<fs::path> paths_;
};

}  // namespace tcap::v4l2
