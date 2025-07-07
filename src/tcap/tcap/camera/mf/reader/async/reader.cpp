#include <memory>

#include "tcap/camera/mf/reader/async/awaitable.hpp"
#include "tcap/camera/mf/reader/async/callback.hpp"
#include "tcap/camera/mf/source.hpp"
#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader.hpp"
#endif

namespace tcap::mf {

ReaderAsyncBox::ReaderAsyncBox(std::unique_ptr<TSampleCallback>&& pSampleCallback) noexcept
    : pSampleCallback_(std::move(pSampleCallback)) {}

std::expected<ReaderAsyncBox, Error> ReaderAsyncBox::create(const SourceBox& sourceBox) noexcept {
    auto pSampleCallback = std::make_unique<TSampleCallback>();

    auto readerBoxRes = ReaderBox::createAsync(sourceBox, pSampleCallback.get());
    if (!readerBoxRes) return std::unexpected{std::move(readerBoxRes.error())};
    auto& readerBox = readerBoxRes.value();

    pSampleCallback->adoptReaderBox(std::move(readerBox));

    return ReaderAsyncBox{std::move(pSampleCallback)};
}

std::expected<void, Error> ReaderAsyncBox::setMediaType(const MediaTypeBox& mediaTypeBox) noexcept {
    return pSampleCallback_->getReaderBox().setMediaType(mediaTypeBox);
}

SampleAwaitable ReaderAsyncBox::sample() noexcept { return SampleAwaitable{pSampleCallback_.get()}; }

}  // namespace tcap::mf
