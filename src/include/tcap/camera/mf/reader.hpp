#pragma once

#include <expected>
#include <memory>

#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/camera/mf/coroutine.hpp"
#include "tcap/camera/mf/source.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class ReaderBox {
    ReaderBox(IMFSourceReader* pReader, std::unique_ptr<SampleCallback>&& pSampleCallback) noexcept;

public:
    ReaderBox(ReaderBox&& rhs) noexcept;
    ~ReaderBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<ReaderBox, Error> create(const SourceBox& sourceBox) noexcept;

    [[nodiscard]] TCAP_API IMFSourceReader* getPReader() const noexcept { return pReader_; }

    [[nodiscard]] TCAP_API SampleAwaitable sample() noexcept;

private:
    IMFSourceReader* pReader_;
    std::unique_ptr<SampleCallback> pSampleCallback_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/reader.cpp"
#endif
