#pragma once

#include <expected>

#include <X11/Xlib.h>

#include "tcap/screen/x11/display/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/screen/x11/window/box.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::x11 {

class ImageBox {
    ImageBox(XImage* image) noexcept;

public:
    ImageBox& operator=(const ImageBox&) = delete;
    ImageBox(const ImageBox&) = delete;
    TCAP_API ImageBox(ImageBox&& rhs) noexcept;
    TCAP_API ImageBox& operator=(ImageBox&& rhs) noexcept;
    TCAP_API ~ImageBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<ImageBox, Error> create(DisplayBox& displayBox,
                                                                        WindowBox& windowBox) noexcept;

    [[nodiscard]] TCAP_API XImage* getImage() const noexcept { return image_; }
    [[nodiscard]] TCAP_API int getWidth() const noexcept { return image_->width; }
    [[nodiscard]] TCAP_API int getHeight() const noexcept { return image_->height; }
    [[nodiscard]] TCAP_API int getBitPerPixel() const noexcept { return image_->bits_per_pixel; }
    [[nodiscard]] TCAP_API int getBytePerLine() const noexcept { return image_->bytes_per_line; }

    TCAP_API void copyTo(std::byte* pData) const noexcept;

private:
    XImage* image_;
};

}  // namespace tcap::x11

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/x11/image/legacy.cpp"
#endif
