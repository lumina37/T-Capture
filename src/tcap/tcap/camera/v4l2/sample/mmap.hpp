#pragma once

#include <expected>

#include "tcap/camera/v4l2/buffer/mmap.hpp"
#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::v4l2 {

class SampleMMap {
public:
    TCAP_API SampleMMap(std::weak_ptr<BufferViewMMap>&& pBufferView, uint64_t timestampNs) noexcept;
    SampleMMap& operator=(const SampleMMap&) = delete;
    SampleMMap(const SampleMMap&) = delete;
    TCAP_API SampleMMap(SampleMMap&& rhs) noexcept = default;
    TCAP_API SampleMMap& operator=(SampleMMap&& rhs) noexcept = default;

    [[nodiscard]] TCAP_API std::weak_ptr<BufferViewMMap> take() noexcept;

    [[nodiscard]] TCAP_API uint64_t getTimestampMs() const noexcept { return timestampMs_; }
    [[nodiscard]] TCAP_API std::expected<void, Error> copyTo(std::byte* pData) const noexcept;

private:
    std::weak_ptr<BufferViewMMap> pBufferView_;
    uint64_t timestampMs_;
};

}  // namespace tcap::v4l2
