#pragma once

#include <cstdint>
#include <expected>

#include <atlbase.h>
#include <mfobjects.h>

#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class SampleBox {
    SampleBox(IMFSample* pSample, DWORD streamIndex, LONGLONG timestamp) noexcept;

public:
    SampleBox(const SampleBox&) = delete;
    TCAP_API SampleBox(SampleBox&& rhs) noexcept;
    TCAP_API SampleBox& operator=(SampleBox&& rhs) noexcept;
    TCAP_API ~SampleBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<SampleBox, Error> create(IMFSample* pSample, DWORD streamIndex,
                                                                         LONGLONG timestamp) noexcept;

    [[nodiscard]] TCAP_API IMFSample* getPSample() const noexcept { return pSample_; }
    [[nodiscard]] TCAP_API uint32_t getStreamIndex() const noexcept { return streamIndex_; }
    [[nodiscard]] TCAP_API int64_t getTimeStamp() const noexcept { return timestamp_; }

private:
    IMFSample* pSample_;
    DWORD streamIndex_;
    LONGLONG timestamp_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/sample.cpp"
#endif
