#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/camera/mf/sample.hpp"
#include "tcap/camera/mf/source.hpp"
#include "tcap/helper/error.hpp"
#include "tcap/helper/mf/attributes.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader/core.hpp"
#endif

namespace tcap::mf {

ReaderBox::ReaderBox(IMFSourceReader* pReader) noexcept : pReader_(pReader) {}

ReaderBox::ReaderBox(ReaderBox&& rhs) noexcept : pReader_(std::exchange(rhs.pReader_, nullptr)) {}

ReaderBox& ReaderBox::operator=(ReaderBox&& rhs) noexcept {
    pReader_ = std::exchange(rhs.pReader_, nullptr);
    return *this;
}

ReaderBox::~ReaderBox() noexcept {
    if (pReader_ == nullptr) return;
    pReader_->Release();
    pReader_ = nullptr;
}

std::expected<ReaderBox, Error> ReaderBox::createSync(const SourceBox& sourceBox) noexcept {
    IMFSourceReader* pReader;
    auto pSource = sourceBox.getPSource();
    const HRESULT hr = MFCreateSourceReaderFromMediaSource(pSource, nullptr, &pReader);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "MFCreateSourceReaderFromMediaSource failed"}};
    }
    pReader->AddRef();

    return ReaderBox{pReader};
}

std::expected<ReaderBox, Error> ReaderBox::createAsync(const SourceBox& sourceBox,
                                                       SampleCallbackBase* pSampleCallback) noexcept {
    auto attrsBoxRes = AttributesBox::create(1);
    if (!attrsBoxRes) return std::unexpected{std::move(attrsBoxRes.error())};
    auto& attrsBox = attrsBoxRes.value();

    auto setRes = attrsBox.setUnknown(MF_SOURCE_READER_ASYNC_CALLBACK, pSampleCallback);
    if (!setRes) return std::unexpected{std::move(setRes.error())};

    IMFSourceReader* pReader;
    auto pSource = sourceBox.getPSource();
    const HRESULT hr = MFCreateSourceReaderFromMediaSource(pSource, attrsBox.getPAttributes(), &pReader);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "MFCreateSourceReaderFromMediaSource failed"}};
    }
    pReader->AddRef();

    return ReaderBox{pReader};
}

std::expected<void, Error> ReaderBox::setMediaType(const MediaTypeBox& mediaTypeBox) noexcept {
    IMFMediaType* pMediaType = mediaTypeBox.getPMediaType();

    const HRESULT hr = pReader_->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, pMediaType);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pReader_->SetCurrentMediaType failed"}};
    }
    return {};
}

std::expected<SampleBox, Error> ReaderBox::sampleSync() noexcept {
    DWORD streamIndex, streamFlags;
    LONGLONG timestamp;
    IMFSample* pSample;
    const HRESULT hr = pReader_->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &streamIndex, &streamFlags,
                                            &timestamp, &pSample);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pReader_->ReadSample failed"}};
    }
    if (pSample == nullptr) {
        return std::unexpected{Error{-1, "pSample is nullptr"}};
    }

    SampleBox sampleBox = SampleBox::create(pSample, streamFlags, timestamp).value();

    return sampleBox;
}

std::expected<void, Error> ReaderBox::sampleAsync() noexcept {
    const HRESULT hr =
        pReader_->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, nullptr, nullptr, nullptr, nullptr);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pReader_->ReadSample failed"}};
    }

    return {};
}

}  // namespace tcap::mf
