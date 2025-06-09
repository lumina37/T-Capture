#include <atlbase.h>
#include <mferror.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/camera/mf/media_type.hpp"
#include "tcap/camera/mf/reader.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader_type.hpp"
#endif

namespace tcap::mf {

ReaderTypeBox::ReaderTypeBox(MediaTypeBox&& currentMediaTypeBox,
                             std::vector<MediaTypeBox>&& nativeMediaTypeBoxes) noexcept
    : currentMediaTypeBox_(std::move(currentMediaTypeBox)), nativeMediaTypeBoxes_(std::move(nativeMediaTypeBoxes)) {}

std::expected<ReaderTypeBox, Error> ReaderTypeBox::create(const AsyncReaderBox& readerBox) noexcept {
    HRESULT hr;

    IMFSourceReader* pReader = readerBox.getPReader();

    CComPtr<IMFMediaType> pCurrMediaType;
    hr = pReader->GetCurrentMediaType(0, &pCurrMediaType);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pReader->GetCurrentMediaType failed"}};
    }
    auto currMediaTypeBoxRes = MediaTypeBox::create(std::move(pCurrMediaType));
    if (!currMediaTypeBoxRes) return std::unexpected{std::move(currMediaTypeBoxRes.error())};
    auto& currMediaTypeBox = currMediaTypeBoxRes.value();

    std::vector<MediaTypeBox> nativeMediaTypeBoxes;
    for (int mediaTypeIdx = 0;; mediaTypeIdx++) {
        CComPtr<IMFMediaType> pMediaType;
        hr = pReader->GetNativeMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, mediaTypeIdx, &pMediaType);
        if (hr == MF_E_NO_MORE_TYPES) {
            break;
        }
        if (FAILED(hr)) {
            return std::unexpected{Error{hr, "pReader->GetNativeMediaType failed"}};
        }

        auto nativeMediaTypeBoxRes = MediaTypeBox::create(std::move(pMediaType));
        if (!nativeMediaTypeBoxRes) return std::unexpected{std::move(nativeMediaTypeBoxRes.error())};
        nativeMediaTypeBoxes.push_back(std::move(nativeMediaTypeBoxRes.value()));
    }

    return ReaderTypeBox{std::move(currMediaTypeBox), std::move(nativeMediaTypeBoxes)};
}

}  // namespace tcap::mf
