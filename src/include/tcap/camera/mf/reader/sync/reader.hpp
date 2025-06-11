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

class ReaderSyncBox {
    ReaderSyncBox(IMFSourceReader* pReader) noexcept;

public:
    ReaderSyncBox(const ReaderSyncBox&) = delete;
    ReaderSyncBox& operator=(const ReaderSyncBox&) = delete;
    TCAP_API ReaderSyncBox(ReaderSyncBox&& rhs) noexcept;
    TCAP_API ReaderSyncBox& operator=(ReaderSyncBox&& rhs) noexcept;
    TCAP_API ~ReaderSyncBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<ReaderSyncBox, Error> create(const SourceBox& sourceBox) noexcept;

    [[nodiscard]] TCAP_API IMFSourceReader* getPReader() const noexcept { return pReader_; }

    [[nodiscard]] TCAP_API std::expected<void, Error> setMediaType(const MediaTypeBox& mediaTypeBox) noexcept;
    [[nodiscard]] TCAP_API std::expected<SampleBox, Error> sample() noexcept;

private:
    IMFSourceReader* pReader_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/reader/sync/reader.cpp"
#endif
