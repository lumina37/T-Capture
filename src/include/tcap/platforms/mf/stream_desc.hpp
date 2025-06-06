#pragma once

#include <expected>
#include <vector>

#include <mfapi.h>
#include <mfidl.h>

#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"
#include "tcap/platforms/mf/media_type.hpp"

namespace tcap::mf {

enum class StreamMajorType {
    eUnknown = 0,
    eVideo = 1,
    eAudio = 2,
};

static StreamMajorType mapGuidToStreamMajorType(const GUID& guid) {
    if (guid == MFMediaType_Video) return StreamMajorType::eVideo;
    if (guid == MFMediaType_Audio) return StreamMajorType::eAudio;
    return StreamMajorType::eUnknown;
}

class StreamDescBox {
    StreamDescBox(IMFStreamDescriptor* pStreamDesc, GUID majorType,
                  std::vector<MediaTypeBox>&& mediaTypeBoxes) noexcept;

public:
    StreamDescBox(StreamDescBox&& rhs) noexcept;
    ~StreamDescBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<StreamDescBox, Error> create(IMFStreamDescriptor* pStreamDesc) noexcept;

    [[nodiscard]] TCAP_API IMFStreamDescriptor* getPStreamDesc() const noexcept { return pStreamDesc_; }
    [[nodiscard]] TCAP_API GUID getMajorTypeGuid() const noexcept { return majorTypeGuid_; }
    [[nodiscard]] TCAP_API StreamMajorType getMajorType() const noexcept { return majorType_; }
    [[nodiscard]] TCAP_API const std::vector<MediaTypeBox>& getMediaTypeBoxes() const noexcept {
        return mediaTypeBoxes_;
    }
    [[nodiscard]] TCAP_API std::vector<MediaTypeBox>& getMediaTypeBoxes() noexcept { return mediaTypeBoxes_; }

private:
    IMFStreamDescriptor* pStreamDesc_;
    GUID majorTypeGuid_;
    StreamMajorType majorType_;
    std::vector<MediaTypeBox> mediaTypeBoxes_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/platforms/mf/stream_desc.cpp"
#endif
