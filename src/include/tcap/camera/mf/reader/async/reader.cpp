#include <memory>

#include <atlbase.h>
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

ReaderAsyncBox::ReaderAsyncBox(CComPtr<IMFSourceReader>&& pReader,
                               std::unique_ptr<SampleCallback>&& pSampleCallback) noexcept
    : pReader_(std::move(pReader)), pSampleCallback_(std::move(pSampleCallback)) {}

ReaderAsyncBox::~ReaderAsyncBox() noexcept {
    // if (pSampleCallback_ == nullptr) return;
    // pSampleCallback_->Release();
    // pSampleCallback_ = nullptr;
}

std::expected<ReaderAsyncBox, Error> ReaderAsyncBox::create(const SourceBox& sourceBox) noexcept {
    auto attrsBoxRes = AttributesBox::create(1);
    if (!attrsBoxRes) return std::unexpected{std::move(attrsBoxRes.error())};
    auto& attrsBox = attrsBoxRes.value();

    auto pSampleCallback = std::make_unique<SampleCallback>();
    auto setRes = attrsBox.setUnknown(MF_SOURCE_READER_ASYNC_CALLBACK, pSampleCallback.get());
    if (!setRes) return std::unexpected{std::move(setRes.error())};

    CComPtr<IMFSourceReader> pReader;
    auto pSource = sourceBox.getPSource();
    const HRESULT hr = MFCreateSourceReaderFromMediaSource(pSource, attrsBox.getPAttributes(), &pReader);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "MFCreateSourceReaderFromMediaSource failed"}};
    }

    // without this `AddRef`, the ref count is only 2
    // for other `CComPtr` like `pBuffer`, the ref count shall be 3
    // so we add another `AddRef` here
    (*pReader).AddRef();
    pSampleCallback->setPReader(pReader);

    return ReaderAsyncBox{std::move(pReader), std::move(pSampleCallback)};
}

SampleAwaitable ReaderAsyncBox::sample() noexcept { return SampleAwaitable{pSampleCallback_.get()}; }

}  // namespace tcap::mf
