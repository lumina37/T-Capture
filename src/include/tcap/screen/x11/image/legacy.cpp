#include <cstring>
#include <expected>
#include <utility>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "tcap/screen/x11/display/box.hpp"
#include "tcap/screen/x11/window/box.hpp"
#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/x11/image/legacy.hpp"
#endif

namespace tcap::x11 {

ImageBox::ImageBox(XImage* pImage) noexcept : pImage_(pImage) {}

ImageBox::ImageBox(ImageBox&& rhs) noexcept : pImage_(std::exchange(rhs.pImage_, nullptr)) {}

ImageBox& ImageBox::operator=(ImageBox&& rhs) noexcept {
    pImage_ = std::exchange(rhs.pImage_, nullptr);
    return *this;
}

ImageBox::~ImageBox() noexcept {
    if (pImage_ == nullptr) return;
    XDestroyImage(pImage_);
    pImage_ = nullptr;
}

std::expected<ImageBox, Error> ImageBox::create(DisplayBox& displayBox, WindowBox& windowBox) noexcept {
    Display* pDisplay = displayBox.getPDisplay();
    Window window = windowBox.getWindow();
    const int width = windowBox.getWidth();
    const int height = windowBox.getHeight();

    XImage* pImage = XGetImage(pDisplay, window, 0, 0, width, height, AllPlanes, ZPixmap);
    if (pImage == nullptr) {
        return std::unexpected{Error{ECate::eX11, 0, "XGetImage failed"}};
    }

    return ImageBox{pImage};
}

void ImageBox::copyTo(std::byte* pData) const noexcept {
    const int bufferSize = pImage_->width * pImage_->height * (pImage_->bits_per_pixel / 8);
    std::memcpy(pData, pImage_->data, bufferSize);
}

}  // namespace tcap::x11
