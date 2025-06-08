#pragma once

#include <expected>

#include <mfobjects.h>

#include "tcap/camera/mf/sample.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class BufferBox {
    BufferBox(IMFMediaBuffer* pBuffer) noexcept;

public:
    TCAP_API BufferBox(BufferBox&& rhs) noexcept;
    TCAP_API ~BufferBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<BufferBox, Error> create(SampleBox& sampleBox) noexcept;

    [[nodiscard]] TCAP_API IMFMediaBuffer* getPBuffer() const noexcept { return pBuffer_; }
    [[nodiscard]] TCAP_API std::expected<void, Error> copyTo(std::byte* pData) const noexcept;

private:
    IMFMediaBuffer* pBuffer_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/buffer.cpp"
#endif
