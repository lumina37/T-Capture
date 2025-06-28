#pragma once

#include <sys/ipc.h>
#include <sys/shm.h>
#include <expected>
#include <memory>

#include <X11/Xlib.h>

#include "tcap/screen/x11/display/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/screen/x11/window/box.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::x11 {

class ImageShmBox {
    ImageShmBox(std::shared_ptr<DisplayBox>&& pDisplayBox, XImage* image, const XShmSegmentInfo& shmInfo) noexcept;

public:
    ImageShmBox& operator=(const ImageShmBox&) = delete;
    ImageShmBox(const ImageShmBox&) = delete;
    TCAP_API ImageShmBox(ImageShmBox&& rhs) noexcept;
    TCAP_API ImageShmBox& operator=(ImageShmBox&& rhs) noexcept;
    TCAP_API ~ImageShmBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<ImageShmBox, Error> create(std::shared_ptr<DisplayBox> pDisplayBox,
                                                                           WindowBox& windowBox) noexcept;

    [[nodiscard]] TCAP_API std::expected<void, Error> fetch(WindowBox& windowBox) noexcept;

    [[nodiscard]] TCAP_API XImage* getImage() const noexcept { return image_; }
    [[nodiscard]] TCAP_API int getWidth() const noexcept { return image_->width; }
    [[nodiscard]] TCAP_API int getHeight() const noexcept { return image_->height; }
    [[nodiscard]] TCAP_API int getBitPerPixel() const noexcept { return image_->bits_per_pixel; }
    [[nodiscard]] TCAP_API int getBytePerLine() const noexcept { return image_->bytes_per_line; }

    TCAP_API void copyTo(std::byte* pData) const noexcept;

private:
    std::shared_ptr<DisplayBox> pDisplayBox_;
    XImage* image_;
    XShmSegmentInfo shmInfo_;
};

}  // namespace tcap::x11

#ifdef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/x11/image/shared.cpp"
#endif
