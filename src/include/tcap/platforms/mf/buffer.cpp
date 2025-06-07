#include <mfobjects.h>

#include "tcap/helper/error.hpp"
#include "tcap/platforms/mf/device/box.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/buffer.hpp"
#endif

namespace tcap::mf {

BufferBox::BufferBox(IMFMediaBuffer* pBuffer) noexcept : pBuffer_(pBuffer) {}

BufferBox::BufferBox(BufferBox&& rhs) noexcept : pBuffer_(std::exchange(rhs.pBuffer_, nullptr)) {}

BufferBox::~BufferBox() noexcept {
    if (pBuffer_ == nullptr) return;
    pBuffer_->Release();
    pBuffer_ = nullptr;
}

std::expected<BufferBox, Error> BufferBox::create(SampleBox& sampleBox) noexcept {
    HRESULT hr;

    IMFSample* pSample = sampleBox.getPSample();

    IMFMediaBuffer* pBuffer;
    hr = pSample->ConvertToContiguousBuffer(&pBuffer);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pSample->ConvertToContiguousBuffer failed"}};
    }

    return BufferBox{pBuffer};
}

std::expected<void, Error> BufferBox::copyTo(std::byte* pData) const noexcept {
    HRESULT hr;

    BYTE* pFirstRow;
    DWORD maxLen, currLen;
    hr = pBuffer_->Lock(&pFirstRow, &maxLen, &currLen);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pBuffer_->Lock failed"}};
    }

    std::memcpy(pData, pFirstRow, currLen);

    hr = pBuffer_->Unlock();
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pBuffer_->Unlock failed"}};
    }

    return {};
}

}  // namespace tcap::mf
