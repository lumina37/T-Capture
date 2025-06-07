#pragma once

#include <expected>

#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"
#include "tcap/platforms/mf/sample.hpp"
#include "tcap/platforms/mf/source.hpp"

namespace tcap::mf {

class ReaderBox {
    ReaderBox(IMFSourceReader* pReader) noexcept;

public:
    ReaderBox(ReaderBox&& rhs) noexcept;
    ~ReaderBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<ReaderBox, Error> create(const SourceBox& sourceBox) noexcept;

    [[nodiscard]] TCAP_API IMFSourceReader* getPReader() const noexcept { return pReader_; }

    [[nodiscard]] TCAP_API std::expected<SampleBox, Error> blockingSample() noexcept;

private:
    IMFSourceReader* pReader_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/reader.cpp"
#endif
