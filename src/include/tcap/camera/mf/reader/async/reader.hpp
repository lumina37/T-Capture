#pragma once

#include <expected>
#include <memory>

#include "tcap/camera/mf/media_type.hpp"
#include "tcap/camera/mf/reader/async/awaitable.hpp"
#include "tcap/camera/mf/reader/core.hpp"
#include "tcap/camera/mf/source.hpp"
#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::mf {

class ReaderAsyncBox {
    ReaderAsyncBox(std::unique_ptr<SampleCallback>&& pSampleCallback) noexcept;

public:
    ReaderAsyncBox(const ReaderAsyncBox&) = delete;
    ReaderAsyncBox& operator=(const ReaderAsyncBox&) = delete;
    TCAP_API ReaderAsyncBox(ReaderAsyncBox&& rhs) noexcept = default;
    TCAP_API ReaderAsyncBox& operator=(ReaderAsyncBox&& rhs) noexcept = default;

    [[nodiscard]] TCAP_API static std::expected<ReaderAsyncBox, Error> create(const SourceBox& sourceBox) noexcept;

    [[nodiscard]] TCAP_API const ReaderBox& getReaderBox() const noexcept { return pSampleCallback_->getReaderBox(); }
    [[nodiscard]] TCAP_API ReaderBox& getReaderBox() noexcept { return pSampleCallback_->getReaderBox(); }

    [[nodiscard]] TCAP_API std::expected<void, Error> setMediaType(const MediaTypeBox& mediaTypeBox) noexcept;
    [[nodiscard]] TCAP_API SampleAwaitable_<SampleCallback> sample() noexcept;

private:
    std::unique_ptr<SampleCallback> pSampleCallback_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader/async/reader.cpp"
#endif
