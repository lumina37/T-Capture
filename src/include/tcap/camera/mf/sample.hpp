#pragma once

#include <cstdint>
#include <expected>

#include <mfobjects.h>

#include "tcap/common/defines.h"
#include "tcap/utils/error.hpp"

namespace tcap::mf {

class SampleBox {
    SampleBox(IMFSample* pSample, DWORD streamFlags, LONGLONG timestamp) noexcept;

public:
    SampleBox(const SampleBox&) = delete;
    SampleBox& operator=(const SampleBox&) = delete;
    TCAP_API SampleBox(SampleBox&& rhs) noexcept;
    TCAP_API SampleBox& operator=(SampleBox&& rhs) noexcept;
    TCAP_API ~SampleBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<SampleBox, Error> create(IMFSample* pSample, DWORD streamFlags,
                                                                         LONGLONG timestamp) noexcept;

    [[nodiscard]] TCAP_API IMFSample* getPSample() const noexcept { return pSample_; }
    [[nodiscard]] TCAP_API uint32_t getStreamFlags() const noexcept { return streamFlags_; }
    [[nodiscard]] TCAP_API int64_t getTimeStamp() const noexcept { return timestamp_; }

private:
    IMFSample* pSample_;
    DWORD streamFlags_;
    LONGLONG timestamp_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/sample.cpp"
#endif
