#include "tcap/camera/mf/sample.hpp"
#include "tcap/camera/mf/source.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader/sync/reader.hpp"
#endif

namespace tcap::mf {

ReaderSync::ReaderSync(ReaderBox&& readerBox) noexcept : readerBox_(std::move(readerBox)) {}

std::expected<ReaderSync, Error> ReaderSync::create(const SourceBox& sourceBox) noexcept {
    auto readerBoxRes = ReaderBox::createSync(sourceBox);
    if (!readerBoxRes) return std::unexpected{std::move(readerBoxRes.error())};
    auto& readerBox = readerBoxRes.value();

    return ReaderSync{std::move(readerBox)};
}

std::expected<void, Error> ReaderSync::setMediaType(const MediaTypeBox& mediaTypeBox) noexcept {
    return readerBox_.setMediaType(mediaTypeBox);
}

std::expected<SampleBox, Error> ReaderSync::sample() noexcept { return readerBox_.sampleSync(); }

}  // namespace tcap::mf
