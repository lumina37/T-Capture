#include <memory>

#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/camera/mf/callback.hpp"
#include "tcap/camera/mf/coroutine.hpp"
#include "tcap/camera/mf/source.hpp"
#include "tcap/helper/error.hpp"
#include "tcap/helper/mf/attributes.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader.hpp"
#endif

namespace tcap::mf {

ReaderBox::ReaderBox(IMFSourceReader* pReader, std::unique_ptr<SampleCallback>&& pSampleCallback) noexcept
    : pReader_(pReader), pSampleCallback_(std::move(pSampleCallback)) {}

ReaderBox::ReaderBox(ReaderBox&& rhs) noexcept
    : pReader_(std::exchange(rhs.pReader_, nullptr)), pSampleCallback_(std::move(rhs.pSampleCallback_)) {}

ReaderBox::~ReaderBox() noexcept {
    if (pReader_ == nullptr) return;
    pSampleCallback_->Release();
    pSampleCallback_ = nullptr;
    pReader_->Release();
    pReader_ = nullptr;
}

std::expected<ReaderBox, Error> ReaderBox::create(const SourceBox& sourceBox) noexcept {
    auto attrsBoxRes = AttributesBox::create(1);
    if (!attrsBoxRes) return std::unexpected{std::move(attrsBoxRes.error())};
    auto& attrsBox = attrsBoxRes.value();

    auto pSampleCallback = std::make_unique<SampleCallback>();
    pSampleCallback->AddRef();
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

    return ReaderBox{pReader, std::move(pSampleCallback)};
}

SampleAwaitable ReaderBox::sample() noexcept { return SampleAwaitable{pSampleCallback_.get()}; }

}  // namespace tcap::mf
