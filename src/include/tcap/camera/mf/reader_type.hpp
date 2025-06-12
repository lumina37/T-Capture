#pragma once

#include <expected>
#include <vector>

#include <mferror.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/camera/mf/media_type.hpp"
#include "tcap/camera/mf/reader/concepts.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class ReaderTypeBox {
    ReaderTypeBox(MediaTypeBox&& currentMediaTypeBox, std::vector<MediaTypeBox>&& nativeMediaTypeBoxes) noexcept;

public:
    ReaderTypeBox(const ReaderTypeBox&) = delete;
    ReaderTypeBox& operator=(const ReaderTypeBox&) = delete;
    TCAP_API ReaderTypeBox(ReaderTypeBox&&) noexcept = default;
    TCAP_API ReaderTypeBox& operator=(ReaderTypeBox&&) noexcept = default;

    template <CSupportGetPReader TReaderBox>
    [[nodiscard]] static std::expected<ReaderTypeBox, Error> create(const TReaderBox& readerBox) noexcept;

    [[nodiscard]] TCAP_API const std::vector<MediaTypeBox>& getNativeMediaTypeBoxes() const noexcept {
        return nativeMediaTypeBoxes_;
    }
    [[nodiscard]] TCAP_API const MediaTypeBox& getCurrentMediaTypeBox() const noexcept { return currentMediaTypeBox_; }

private:
    MediaTypeBox currentMediaTypeBox_;
    std::vector<MediaTypeBox> nativeMediaTypeBoxes_;
};

template <CSupportGetPReader TReaderBox>
std::expected<ReaderTypeBox, Error> ReaderTypeBox::create(const TReaderBox& readerBox) noexcept {
    HRESULT hr;

    IMFSourceReader* pReader = readerBox.getPReader();

    IMFMediaType* pCurrMediaType;
    hr = pReader->GetCurrentMediaType(0, &pCurrMediaType);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pReader->GetCurrentMediaType failed"}};
    }
    auto currMediaTypeBoxRes = MediaTypeBox::create(pCurrMediaType);
    if (!currMediaTypeBoxRes) return std::unexpected{std::move(currMediaTypeBoxRes.error())};
    auto& currMediaTypeBox = currMediaTypeBoxRes.value();

    std::vector<MediaTypeBox> nativeMediaTypeBoxes;
    for (int mediaTypeIdx = 0;; mediaTypeIdx++) {
        IMFMediaType* pMediaType;
        hr = pReader->GetNativeMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, mediaTypeIdx, &pMediaType);
        if (hr == MF_E_NO_MORE_TYPES) {
            break;
        }
        if (FAILED(hr)) {
            return std::unexpected{Error{hr, "pReader->GetNativeMediaType failed"}};
        }

        auto nativeMediaTypeBoxRes = MediaTypeBox::create(pMediaType);
        if (!nativeMediaTypeBoxRes) return std::unexpected{std::move(nativeMediaTypeBoxRes.error())};
        nativeMediaTypeBoxes.push_back(std::move(nativeMediaTypeBoxRes.value()));
    }

    return ReaderTypeBox{std::move(currMediaTypeBox), std::move(nativeMediaTypeBoxes)};
}

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader_type.cpp"
#endif
