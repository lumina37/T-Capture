#pragma once

#include <expected>
#include <vector>

#include <mfidl.h>

#include "tcap/camera/mf/media_type.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class StreamDescBox {
    StreamDescBox(IMFStreamDescriptor* pStreamDesc, std::vector<MediaTypeBox>&& mediaTypeBoxes) noexcept;

public:
    StreamDescBox(StreamDescBox&& rhs) noexcept;
    ~StreamDescBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<StreamDescBox, Error> create(IMFStreamDescriptor* pStreamDesc) noexcept;

    [[nodiscard]] TCAP_API IMFStreamDescriptor* getPStreamDesc() const noexcept { return pStreamDesc_; }

    [[nodiscard]] TCAP_API const std::vector<MediaTypeBox>& getMediaTypeBoxes() const noexcept {
        return mediaTypeBoxes_;
    }
    [[nodiscard]] TCAP_API std::vector<MediaTypeBox>& getMediaTypeBoxes() noexcept { return mediaTypeBoxes_; }

private:
    IMFStreamDescriptor* pStreamDesc_;
    std::vector<MediaTypeBox> mediaTypeBoxes_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/stream_desc.cpp"
#endif
