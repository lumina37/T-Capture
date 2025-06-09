#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/camera/mf/sample.hpp"
#include "tcap/camera/mf/source.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader/sync/reader.hpp"
#endif

namespace tcap::mf {

ReaderSyncBox::ReaderSyncBox(CComPtr<IMFSourceReader>&& pReader) noexcept : pReader_(std::move(pReader)) {}

std::expected<ReaderSyncBox, Error> ReaderSyncBox::create(const SourceBox& sourceBox) noexcept {
    IMFSourceReader* pReader;
    auto pSource = sourceBox.getPSource();
    const HRESULT hr = MFCreateSourceReaderFromMediaSource(pSource, nullptr, &pReader);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "MFCreateSourceReaderFromMediaSource failed"}};
    }

    return ReaderSyncBox{pReader};
}

std::expected<SampleBox, Error> ReaderSyncBox::sample() noexcept {
    DWORD streamIndex, flags;
    LONGLONG llTimeStamp;
    IMFSample* pSample;
    const HRESULT hr =
        pReader_->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &streamIndex, &flags, &llTimeStamp, &pSample);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pReader_->ReadSample failed"}};
    }
    if (pSample == nullptr) {
        return std::unexpected{Error{-1, "pSample is nullptr"}};
    }

    SampleBox sampleBox = SampleBox::create(pSample).value();

    return sampleBox;
}

}  // namespace tcap::mf
