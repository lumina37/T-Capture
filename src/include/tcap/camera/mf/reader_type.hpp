#pragma once

#include <expected>
#include <vector>

#include "tcap/camera/mf/media_type.hpp"
#include "tcap/camera/mf/reader/core.hpp"
#include "tcap/common/defines.h"
#include "tcap/helper/error.hpp"

namespace tcap::mf {

class ReaderTypeBox {
    ReaderTypeBox(MediaTypeBox&& currentMediaTypeBox, std::vector<MediaTypeBox>&& nativeMediaTypeBoxes) noexcept;

public:
    ReaderTypeBox(const ReaderTypeBox&) = delete;
    ReaderTypeBox& operator=(const ReaderTypeBox&) = delete;
    TCAP_API ReaderTypeBox(ReaderTypeBox&&) noexcept = default;
    TCAP_API ReaderTypeBox& operator=(ReaderTypeBox&&) noexcept = default;

    [[nodiscard]] static std::expected<ReaderTypeBox, Error> create(const ReaderBox& readerBox) noexcept;

    [[nodiscard]] TCAP_API const std::vector<MediaTypeBox>& getNativeMediaTypeBoxes() const noexcept {
        return nativeMediaTypeBoxes_;
    }
    [[nodiscard]] TCAP_API const MediaTypeBox& getCurrentMediaTypeBox() const noexcept { return currentMediaTypeBox_; }

private:
    MediaTypeBox currentMediaTypeBox_;
    std::vector<MediaTypeBox> nativeMediaTypeBoxes_;
};

}  // namespace tcap::mf

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader_type.cpp"
#endif
