#include <expected>
#include <utility>

#include "tcap/camera/v4l2/device/box.hpp"
#include "tcap/camera/v4l2/sample/mmap.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::v4l2 {

SampleMMap::SampleMMap(std::weak_ptr<BufferViewMMap>&& pBufferView, const uint64_t timestampMs) noexcept
    : pBufferView_(std::move(pBufferView)), timestampMs_(timestampMs) {}

std::weak_ptr<BufferViewMMap> SampleMMap::take() noexcept { return std::exchange(pBufferView_, {}); }

std::expected<void, Error> SampleMMap::copyTo(std::byte* pData) const noexcept {
    auto pBufferView = pBufferView_.lock();
    if (pBufferView == nullptr) {
        return std::unexpected{
            Error{ECate::eTCap, ECode::eResourceInvalid, "pBufferView_ is taken or queue is deconstructed"}};
    }

    pBufferView->copyTo(pData);

    return {};
}

}  // namespace tcap::v4l2
