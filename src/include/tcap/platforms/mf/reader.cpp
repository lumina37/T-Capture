#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/helper/error.hpp"
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

}  // namespace tcap::mf
