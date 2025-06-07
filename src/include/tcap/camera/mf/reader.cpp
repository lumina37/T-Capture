#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/camera/mf/async.hpp"
#include "tcap/camera/mf/sample.hpp"
#include "tcap/camera/mf/source.hpp"
#include "tcap/helper/error.hpp"
#include "tcap/helper/mf/attributes.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader.hpp"
#endif

namespace tcap::mf {

ReaderBox::ReaderBox(IMFSourceReader* pReader, SampleCallback* pSampleCallback) noexcept
    : pReader_(pReader), pSampleCallback_(pSampleCallback) {}

ReaderBox::ReaderBox(ReaderBox&& rhs) noexcept
    : pReader_(std::exchange(rhs.pReader_, nullptr)), pSampleCallback_(std::exchange(rhs.pSampleCallback_, nullptr)) {}

ReaderBox::~ReaderBox() noexcept {
    if (pReader_ == nullptr) return;
    pReader_->Release();
    pReader_ = nullptr;
}

std::expected<ReaderBox, Error> ReaderBox::create(const SourceBox& sourceBox) noexcept {
    auto attrsBoxRes = AttributesBox::create(1);
    if (!attrsBoxRes) return std::unexpected{std::move(attrsBoxRes.error())};
    auto& attrsBox = attrsBoxRes.value();

    auto* pSampleCallback = new SampleCallback;
    auto setRes = attrsBox.setUnknown(MF_SOURCE_READER_ASYNC_CALLBACK, pSampleCallback);
    if (!setRes) return std::unexpected{std::move(setRes.error())};

    IMFSourceReader* pReader;
    auto pSource = sourceBox.getPSource();
    const HRESULT hr = MFCreateSourceReaderFromMediaSource(pSource, attrsBox.getPAttributes(), &pReader);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "MFCreateSourceReaderFromMediaSource failed"}};
    }

    pSampleCallback->pReader_ = pReader;

    return ReaderBox{pReader, pSampleCallback};
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

SampleCallback::SampleAwaitable ReaderBox::sample() noexcept { return pSampleCallback_->sample(); }

}  // namespace tcap::mf
