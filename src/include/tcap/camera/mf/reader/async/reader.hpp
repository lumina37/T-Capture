#pragma once

#include <expected>
#include <memory>

#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/camera/mf/reader/async/awaitable.hpp"
#include "tcap/camera/mf/source.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class AsyncReaderBox {
    AsyncReaderBox(IMFSourceReader* pReader, std::unique_ptr<SampleCallback>&& pSampleCallback) noexcept;

public:
    AsyncReaderBox(const AsyncReaderBox&) = delete;
    TCAP_API AsyncReaderBox(AsyncReaderBox&& rhs) noexcept;
    TCAP_API ~AsyncReaderBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<AsyncReaderBox, Error> create(const SourceBox& sourceBox) noexcept;

    [[nodiscard]] TCAP_API IMFSourceReader* getPReader() const noexcept { return pReader_; }

    [[nodiscard]] TCAP_API SampleAwaitable sample() noexcept;

private:
    IMFSourceReader* pReader_;
    std::unique_ptr<SampleCallback> pSampleCallback_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader/async/reader.cpp"
#endif
