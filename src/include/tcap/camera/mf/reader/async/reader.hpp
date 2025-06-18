#pragma once

#include <expected>
#include <memory>

#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/camera/mf/media_type.hpp"
#include "tcap/camera/mf/reader/async/awaitable.hpp"
#include "tcap/camera/mf/source.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class ReaderAsyncBox {
    ReaderAsyncBox(IMFSourceReader* pReader, std::unique_ptr<SampleCallbackBase>&& pSampleCallback) noexcept;

public:
    ReaderAsyncBox(const ReaderAsyncBox&) = delete;
    ReaderAsyncBox& operator=(const ReaderAsyncBox&) = delete;
    TCAP_API ReaderAsyncBox(ReaderAsyncBox&& rhs) noexcept;
    TCAP_API ReaderAsyncBox& operator=(ReaderAsyncBox&& rhs) noexcept;
    TCAP_API ~ReaderAsyncBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<ReaderAsyncBox, Error> create(const SourceBox& sourceBox) noexcept;

    [[nodiscard]] TCAP_API IMFSourceReader* getPReader() const noexcept { return pReader_; }

    [[nodiscard]] TCAP_API std::expected<void, Error> setMediaType(const MediaTypeBox& mediaTypeBox) noexcept;
    [[nodiscard]] TCAP_API SampleAwaitable sample() noexcept;

private:
    IMFSourceReader* pReader_;
    std::unique_ptr<SampleCallbackBase> pSampleCallback_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader/async/reader.cpp"
#endif
