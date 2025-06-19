#pragma once

#include <expected>

#include "tcap/camera/mf/media_type.hpp"
#include "tcap/camera/mf/reader/core.hpp"
#include "tcap/camera/mf/sample.hpp"
#include "tcap/camera/mf/source.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class ReaderSync {
    ReaderSync(ReaderBox&& readerBox) noexcept;

public:
    ReaderSync(const ReaderSync&) = delete;
    ReaderSync& operator=(const ReaderSync&) = delete;
    TCAP_API ReaderSync(ReaderSync&& rhs) noexcept = default;
    TCAP_API ReaderSync& operator=(ReaderSync&& rhs) noexcept = default;

    [[nodiscard]] TCAP_API static std::expected<ReaderSync, Error> create(const SourceBox& sourceBox) noexcept;

    [[nodiscard]] TCAP_API const ReaderBox& getReaderBox() const noexcept { return readerBox_; }
    [[nodiscard]] TCAP_API ReaderBox& getReaderBox() noexcept { return readerBox_; }

    [[nodiscard]] TCAP_API std::expected<void, Error> setMediaType(const MediaTypeBox& mediaTypeBox) noexcept;
    [[nodiscard]] TCAP_API std::expected<SampleBox, Error> sample() noexcept;

private:
    ReaderBox readerBox_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader/sync/reader.cpp"
#endif
