#include <mferror.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/helper/error.hpp"
#include "tcap/platforms/mf/media_type.hpp"
#include "tcap/platforms/mf/reader.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/reader_type.hpp"
#endif

namespace tcap::mf {

ReaderTypeBox::ReaderTypeBox(std::vector<std::vector<MediaTypeBox>>&& mediaTypeBoxesForStreams) noexcept
    : nativeMediaTypeBoxes_(std::move(mediaTypeBoxesForStreams)) {}

std::expected<ReaderTypeBox, Error> ReaderTypeBox::create(const ReaderBox& readerBox) noexcept {
    IMFSourceReader* pReader = readerBox.getPReader();
    std::vector<std::vector<MediaTypeBox>> nativeMediaTypeBoxes;
    for (int streamIdx = 0;; streamIdx++) {
        HRESULT enumReaderHr{};
        std::vector<MediaTypeBox> mediaTypeBoxes;
        for (int mediaTypeIdx = 0;; mediaTypeIdx++) {
            IMFMediaType* pMediaType;
            const HRESULT enumStreamHr = pReader->GetNativeMediaType(streamIdx, mediaTypeIdx, &pMediaType);
            if (enumStreamHr == MF_E_NO_MORE_TYPES) {
                break;
            }
            if (enumStreamHr == MF_E_INVALIDSTREAMNUMBER) {
                enumReaderHr = enumStreamHr;  // break the outer loop
                break;
            }
            if (FAILED(enumStreamHr)) {
                return std::unexpected{Error{enumReaderHr, "pReader->GetNativeMediaType got unexpected error"}};
            }

            auto mediaTypeBoxRes = MediaTypeBox::create(pMediaType);
            if (!mediaTypeBoxRes) return std::unexpected{std::move(mediaTypeBoxRes.error())};
            mediaTypeBoxes.push_back(std::move(mediaTypeBoxRes.value()));
        }

        nativeMediaTypeBoxes.push_back(std::move(mediaTypeBoxes));

        if (enumReaderHr == MF_E_INVALIDSTREAMNUMBER) {
            break;
        }
    }

    return ReaderTypeBox{std::move(nativeMediaTypeBoxes)};
}

}  // namespace tcap::mf
