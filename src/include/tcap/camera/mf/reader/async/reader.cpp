#include <memory>

#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/camera/mf/reader/async/awaitable.hpp"
#include "tcap/camera/mf/reader/async/callback.hpp"
#include "tcap/camera/mf/source.hpp"
#include "tcap/helper/error.hpp"
#include "tcap/helper/mf/attributes.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader.hpp"
#endif

namespace tcap::mf {

ReaderAsyncBox::ReaderAsyncBox(IMFSourceReader* pReader, std::unique_ptr<SampleCallbackBase>&& pSampleCallback) noexcept
    : pReader_(pReader), pSampleCallback_(std::move(pSampleCallback)) {}

ReaderAsyncBox::ReaderAsyncBox(ReaderAsyncBox&& rhs) noexcept
    : pReader_(std::exchange(rhs.pReader_, nullptr)), pSampleCallback_(std::move(rhs.pSampleCallback_)) {}

ReaderAsyncBox& ReaderAsyncBox::operator=(ReaderAsyncBox&& rhs) noexcept {
    pReader_ = std::exchange(rhs.pReader_, nullptr);
    pSampleCallback_ = std::move(rhs.pSampleCallback_);
    return *this;
}

ReaderAsyncBox::~ReaderAsyncBox() noexcept {
    if (pReader_ == nullptr) return;
    pReader_->Release();
    pReader_ = nullptr;
}

std::expected<ReaderAsyncBox, Error> ReaderAsyncBox::create(const SourceBox& sourceBox) noexcept {
    auto attrsBoxRes = AttributesBox::create(1);
    if (!attrsBoxRes) return std::unexpected{std::move(attrsBoxRes.error())};
    auto& attrsBox = attrsBoxRes.value();

    auto pSampleCallback = std::make_unique<SampleCallback>();
    auto setRes = attrsBox.setUnknown(MF_SOURCE_READER_ASYNC_CALLBACK, pSampleCallback.get());
    if (!setRes) return std::unexpected{std::move(setRes.error())};

    IMFSourceReader* pReader;
    auto pSource = sourceBox.getPSource();
    const HRESULT hr = MFCreateSourceReaderFromMediaSource(pSource, attrsBox.getPAttributes(), &pReader);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "MFCreateSourceReaderFromMediaSource failed"}};
    }

    pReader->AddRef();
    pSampleCallback->setPReader(pReader);

    return ReaderAsyncBox{std::move(pReader), std::move(pSampleCallback)};
}

std::expected<void, Error> ReaderAsyncBox::setMediaType(const MediaTypeBox& mediaTypeBox) noexcept {
    IMFMediaType* pMediaType = mediaTypeBox.getPMediaType();

    const HRESULT hr = pReader_->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, pMediaType);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pReader_->SetCurrentMediaType failed"}};
    }
    return {};
}

SampleAwaitable ReaderAsyncBox::sample() noexcept { return SampleAwaitable{(SampleCallback*)pSampleCallback_.get()}; }

}  // namespace tcap::mf
