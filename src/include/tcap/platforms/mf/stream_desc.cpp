#include <mfidl.h>

#include "tcap/helper/error.hpp"
#include "tcap/platforms/mf/source.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/stream_desc.hpp"
#endif

namespace tcap::mf {

StreamDescBox::StreamDescBox(IMFStreamDescriptor* pStreamDesc, GUID majorTypeGuid,
                             std::vector<MediaTypeBox>&& mediaTypeBoxes) noexcept
    : pStreamDesc_(pStreamDesc),
      majorTypeGuid_(majorTypeGuid),
      majorType_(mapGuidToStreamMajorType(majorTypeGuid)),
      mediaTypeBoxes_(std::move(mediaTypeBoxes)) {}

StreamDescBox::StreamDescBox(StreamDescBox&& rhs) noexcept
    : pStreamDesc_(std::exchange(rhs.pStreamDesc_, nullptr)),
      majorType_(rhs.majorType_),
      mediaTypeBoxes_(std::move(rhs.mediaTypeBoxes_)) {}

StreamDescBox::~StreamDescBox() noexcept {
    if (pStreamDesc_ == nullptr) return;
    pStreamDesc_->Release();
    pStreamDesc_ = nullptr;
}

std::expected<StreamDescBox, Error> StreamDescBox::create(IMFStreamDescriptor* pStreamDesc) noexcept {
    HRESULT hr;

    IMFMediaTypeHandler* pMediaTypeHandler;
    hr = pStreamDesc->GetMediaTypeHandler(&pMediaTypeHandler);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pPresentDesc->GetMediaTypeHandler failed"}};
    }

    DWORD mediaTypeCount;
    hr = pMediaTypeHandler->GetMediaTypeCount(&mediaTypeCount);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pMediaTypeHandler->GetMediaTypeCount failed"}};
    }

    GUID majorTypeGuid;
    hr = pMediaTypeHandler->GetMajorType(&majorTypeGuid);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pMediaTypeHandler->GetMajorType failed"}};
    }
    const StreamMajorType majorType = mapGuidToStreamMajorType(majorTypeGuid);

    std::vector<MediaTypeBox> mediaTypeBoxes;
    mediaTypeBoxes.reserve(mediaTypeCount);
    for (DWORD i = 0; i < mediaTypeCount; i++) {
        IMFMediaType* pMediaType;
        hr = pMediaTypeHandler->GetMediaTypeByIndex(i, &pMediaType);
        if (FAILED(hr)) {
            return std::unexpected{Error{hr, "pMediaTypeHandler->GetMediaTypeByIndex failed"}};
        }

        auto mediaTypeBoxRes = MediaTypeBox::create(pMediaType);
        if (!mediaTypeBoxRes) return std::unexpected{std::move(mediaTypeBoxRes.error())};
        mediaTypeBoxes.push_back(std::move(mediaTypeBoxRes.value()));
    }

    return StreamDescBox{pStreamDesc, majorTypeGuid, std::move(mediaTypeBoxes)};
}

}  // namespace tcap::mf
