#include <mfobjects.h>

#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/buffer.hpp"
#endif

namespace tcap::mf {

BufferBox::BufferBox(IMFMediaBuffer* pBuffer) noexcept : pBuffer_(pBuffer) {}

BufferBox::BufferBox(BufferBox&& rhs) noexcept : pBuffer_(std::exchange(rhs.pBuffer_, nullptr)) {}

BufferBox& BufferBox::operator=(BufferBox&& rhs) noexcept {
    pBuffer_ = std::exchange(rhs.pBuffer_, nullptr);
    return *this;
}

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
        return std::unexpected{Error{ECate::eMF, hr}};
    }
    pBuffer->AddRef();

    return BufferBox{std::move(pBuffer)};
}

std::expected<void, Error> BufferBox::copyTo(std::byte* pData) const noexcept {
    HRESULT hr;

    BYTE* pFirstRow;
    DWORD maxLen, currLen;
    hr = pBuffer_->Lock(&pFirstRow, &maxLen, &currLen);
    if (FAILED(hr)) {
        return std::unexpected{Error{ECate::eMF, hr}};
    }

    std::memcpy(pData, pFirstRow, currLen);

    hr = pBuffer_->Unlock();
    if (FAILED(hr)) {
        return std::unexpected{Error{ECate::eMF, hr}};
    }

    return {};
}

}  // namespace tcap::mf
