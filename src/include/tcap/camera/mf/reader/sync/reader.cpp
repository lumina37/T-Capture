#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/camera/mf/sample.hpp"
#include "tcap/camera/mf/source.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader/sync/reader.hpp"
#endif

namespace tcap::mf {

ReaderSyncBox::ReaderSyncBox(IMFSourceReader* pReader) noexcept : pReader_(pReader) {}

ReaderSyncBox::ReaderSyncBox(ReaderSyncBox&& rhs) noexcept : pReader_(std::exchange(rhs.pReader_, nullptr)) {}

ReaderSyncBox& ReaderSyncBox::operator=(ReaderSyncBox&& rhs) noexcept {
    pReader_ = std::exchange(rhs.pReader_, nullptr);
    return *this;
}

ReaderSyncBox::~ReaderSyncBox() noexcept {
    if (pReader_ == nullptr) return;
    pReader_->Release();
    pReader_ = nullptr;
}

std::expected<ReaderSyncBox, Error> ReaderSyncBox::create(const SourceBox& sourceBox) noexcept {
    IMFSourceReader* pReader;
    auto pSource = sourceBox.getPSource();
    const HRESULT hr = MFCreateSourceReaderFromMediaSource(pSource, nullptr, &pReader);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "MFCreateSourceReaderFromMediaSource failed"}};
    }
    pReader->AddRef();

    return ReaderSyncBox{pReader};
}

std::expected<void, Error> ReaderSyncBox::setMediaType(const MediaTypeBox& mediaTypeBox) noexcept {
    IMFMediaType* pMediaType = mediaTypeBox.getPMediaType();

    const HRESULT hr = pReader_->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, pMediaType);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pReader_->SetCurrentMediaType failed"}};
    }
    return {};
}

std::expected<SampleBox, Error> ReaderSyncBox::sample() noexcept {
    DWORD streamIndex, streamFlags;
    LONGLONG timestamp;
    IMFSample* pSample;
    const HRESULT hr =
        pReader_->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &streamIndex, &streamFlags, &timestamp, &pSample);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pReader_->ReadSample failed"}};
    }
    if (pSample == nullptr) {
        return std::unexpected{Error{-1, "pSample is nullptr"}};
    }

    SampleBox sampleBox = SampleBox::create(pSample, streamFlags, timestamp).value();

    return sampleBox;
}

}  // namespace tcap::mf
