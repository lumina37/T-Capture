#include <cstring>
#include <expected>
#include <utility>

#include <X11/Xlib.h>

#include "tcap/screen/x11/display.hpp"
#include "tcap/screen/x11/window/box.hpp"
#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/x11/image.hpp"
#endif

namespace tcap::x11 {

ImageBox::ImageBox(XImage* image) noexcept : image_(image) {}

ImageBox::ImageBox(ImageBox&& rhs) noexcept : image_(std::exchange(rhs.image_, nullptr)) {}

ImageBox& ImageBox::operator=(ImageBox&& rhs) noexcept {
    image_ = std::exchange(rhs.image_, nullptr);
    return *this;
}

ImageBox::~ImageBox() noexcept {
    if (image_ == nullptr) return;
    image_ = nullptr;
}

std::expected<ImageBox, Error> ImageBox::create(DisplayBox& displayBox, WindowBox& windowBox) noexcept {
    Display* display = displayBox.getDisplay();
    Window window = windowBox.getWindow();
    const int width = windowBox.getWidth();
    const int height = windowBox.getHeight();

    XImage* image = XGetImage(display, window, 0, 0, width, height, AllPlanes, ZPixmap);
    if (image == nullptr) {
        return std::unexpected{Error{ECate::eX11, 0}};
    }

    return ImageBox{image};
}

void ImageBox::copyTo(std::byte* pData) const noexcept {
    const int bufferSize = image_->width * image_->height * (image_->bits_per_pixel / 8);
    std::memcpy(pData, image_->data, bufferSize);
}

}  // namespace tcap::x11
