#include <atlbase.h>
#include <mfobjects.h>

#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/buffer.hpp"
#endif

namespace tcap::mf {

BufferBox::BufferBox(CComPtr<IMFMediaBuffer>&& pBuffer) noexcept : pBuffer_(pBuffer) {}

std::expected<BufferBox, Error> BufferBox::create(SampleBox& sampleBox) noexcept {
    HRESULT hr;

    IMFSample* pSample = sampleBox.getPSample();

    IMFMediaBuffer* pBuffer;
    hr = pSample->ConvertToContiguousBuffer(&pBuffer);
    if (FAILED(hr)) {
        return std::unexpected{Error{hr, "pSample->ConvertToContiguousBuffer failed"}};
    }
    pBuffer->AddRef();

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
