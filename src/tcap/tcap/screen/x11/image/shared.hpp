#pragma once

#include <expected>
#include <memory>

#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>

#include "tcap/common/defines.h"
#include "tcap/screen/x11/display/box.hpp"
#include "tcap/screen/x11/screen.hpp"
#include "tcap/screen/x11/window/box.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::x11 {

class ImageShmBox {
    ImageShmBox(std::shared_ptr<DisplayBox>&& pDisplayBox, XImage* pImage,
                std::unique_ptr<XShmSegmentInfo>&& shmInfo) noexcept;

public:
    ImageShmBox& operator=(const ImageShmBox&) = delete;
    ImageShmBox(const ImageShmBox&) = delete;
    TCAP_API ImageShmBox(ImageShmBox&& rhs) noexcept;
    TCAP_API ImageShmBox& operator=(ImageShmBox&& rhs) noexcept;
    TCAP_API ~ImageShmBox() noexcept;

    [[nodiscard]] TCAP_API static std::expected<ImageShmBox, Error> create(std::shared_ptr<DisplayBox> pDisplayBox,
                                                                           WindowBox& windowBox) noexcept;

    [[nodiscard]] TCAP_API std::expected<void, Error> fetch(WindowBox& windowBox) noexcept;

    [[nodiscard]] TCAP_API XImage* getPImage() const noexcept { return pImage_; }
    [[nodiscard]] TCAP_API int getWidth() const noexcept { return pImage_->width; }
    [[nodiscard]] TCAP_API int getHeight() const noexcept { return pImage_->height; }
    [[nodiscard]] TCAP_API int getBitPerPixel() const noexcept { return pImage_->bits_per_pixel; }
    [[nodiscard]] TCAP_API int getBytePerLine() const noexcept { return pImage_->bytes_per_line; }

    TCAP_API void copyTo(std::byte* pData) const noexcept;

private:
    std::shared_ptr<DisplayBox> pDisplayBox_;
    XImage* pImage_;
    std::unique_ptr<XShmSegmentInfo> shmInfo_;
};

}  // namespace tcap::x11
