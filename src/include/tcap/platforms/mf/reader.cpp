#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/helper/error.hpp"
#include "tcap/platforms/mf/sample.hpp"
#include "tcap/platforms/mf/source.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/reader.hpp"
#endif

namespace tcap::mf {

ReaderBox::ReaderBox(IMFSourceReader* pReader) noexcept : pReader_(pReader) {}

ReaderBox::ReaderBox(ReaderBox&& rhs) noexcept : pReader_(std::exchange(rhs.pReader_, nullptr)) {}

ReaderBox::~ReaderBox() noexcept {
    if (pReader_ == nullptr) return;
    pReader_->Release();
    pReader_ = nullptr;
}

std::expected<ReaderBox, Error> ReaderBox::create(const SourceBox& sourceBox) noexcept {
    IMFSourceReader* pReader;
    auto pSource = sourceBox.getPSource();
    const HRESULT hr = MFCreateSourceReaderFromMediaSource(pSource, nullptr, &pReader);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "MFCreateSourceReaderFromMediaSource failed"}};
    }

    return ReaderBox{pReader};
}

std::expected<SampleBox, Error> ReaderBox::blockingSample() noexcept {
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
