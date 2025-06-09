#pragma once

#include <expected>

#include <atlbase.h>
#include <mfobjects.h>

#include "tcap/camera/mf/sample.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class BufferBox {
    BufferBox(CComPtr<IMFMediaBuffer>&& pBuffer) noexcept;

public:
    BufferBox(const BufferBox&) = delete;
    TCAP_API BufferBox(BufferBox&&) noexcept = default;

    [[nodiscard]] TCAP_API static std::expected<BufferBox, Error> create(SampleBox& sampleBox) noexcept;

    [[nodiscard]] TCAP_API IMFMediaBuffer* getPBuffer() const noexcept { return pBuffer_; }
    [[nodiscard]] TCAP_API std::expected<void, Error> copyTo(std::byte* pData) const noexcept;

private:
    CComPtr<IMFMediaBuffer> pBuffer_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/buffer.cpp"
#endif
