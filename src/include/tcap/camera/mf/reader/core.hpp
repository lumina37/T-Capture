#pragma once

#include <expected>

#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/camera/mf/media_type.hpp"
#include "tcap/camera/mf/sample.hpp"
#include "tcap/camera/mf/source.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class ReaderBox {
    ReaderBox(IMFSourceReader* pReader) noexcept;

public:
    TCAP_API ReaderBox() = default;
    ReaderBox(const ReaderBox&) = delete;
    ReaderBox& operator=(const ReaderBox&) = delete;
    TCAP_API ReaderBox(ReaderBox&& rhs) noexcept;
    TCAP_API ReaderBox& operator=(ReaderBox&& rhs) noexcept;
    TCAP_API ~ReaderBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<ReaderBox, Error> createSync(const SourceBox& sourceBox) noexcept;
    [[nodiscard]] TCAP_API static std::expected<ReaderBox, Error> createAsync(
        const SourceBox& sourceBox, IMFSourceReaderCallback* pCallback) noexcept;

    [[nodiscard]] TCAP_API IMFSourceReader* getPReader() const noexcept { return pReader_; }

    [[nodiscard]] TCAP_API std::expected<void, Error> setMediaType(const MediaTypeBox& mediaTypeBox) noexcept;
    [[nodiscard]] TCAP_API std::expected<SampleBox, Error> sampleSync() noexcept;
    [[nodiscard]] TCAP_API std::expected<void, Error> sampleAsync() noexcept;

private:
    IMFSourceReader* pReader_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader/core.cpp"
#endif
