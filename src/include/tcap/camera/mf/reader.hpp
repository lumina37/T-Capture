#pragma once

#include <expected>

#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/camera/mf/async.hpp"
#include "tcap/camera/mf/sample.hpp"
#include "tcap/camera/mf/source.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class ReaderBox {
    ReaderBox(IMFSourceReader* pReader, SampleCallback* pSampleCallback) noexcept;

public:
    ReaderBox(ReaderBox&& rhs) noexcept;
    ~ReaderBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<ReaderBox, Error> create(const SourceBox& sourceBox) noexcept;

    [[nodiscard]] TCAP_API IMFSourceReader* getPReader() const noexcept { return pReader_; }

    [[nodiscard]] TCAP_API std::expected<SampleBox, Error> blockingSample() noexcept;
    [[nodiscard]] TCAP_API SampleCallback::SampleAwaitable sample() noexcept;

private:
    IMFSourceReader* pReader_;
    SampleCallback* pSampleCallback_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/reader.cpp"
#endif
