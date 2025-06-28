#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <expected>
#include <utility>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>

#include "tcap/screen/x11/display/box.hpp"
#include "tcap/screen/x11/window/box.hpp"
#include "tcap/utils/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/screen/x11/image/shared.hpp"
#endif

namespace tcap::x11 {

ImageShmBox::ImageShmBox(std::shared_ptr<DisplayBox>&& pDisplayBox, XImage* image,
                         std::unique_ptr<XShmSegmentInfo>&& shmInfo) noexcept
    : pDisplayBox_(std::move(pDisplayBox)), image_(image), shmInfo_(std::move(shmInfo)) {}

ImageShmBox::ImageShmBox(ImageShmBox&& rhs) noexcept
    : pDisplayBox_(std::move(rhs.pDisplayBox_)),
      image_(std::exchange(rhs.image_, nullptr)),
      shmInfo_(std::move(rhs.shmInfo_)) {}

ImageShmBox& ImageShmBox::operator=(ImageShmBox&& rhs) noexcept {
    pDisplayBox_ = std::move(rhs.pDisplayBox_);
    image_ = std::exchange(rhs.image_, nullptr);
    shmInfo_ = std::move(rhs.shmInfo_);
    return *this;
}

ImageShmBox::~ImageShmBox() noexcept {
    if (image_ == nullptr) return;
    Display* display = pDisplayBox_->getDisplay();
    XShmDetach(display, shmInfo_.get());
    XDestroyImage(image_);
    shmdt(shmInfo_->shmaddr);
    shmctl(shmInfo_->shmid, IPC_RMID, nullptr);
    image_ = nullptr;
}

std::expected<ImageShmBox, Error> ImageShmBox::create(std::shared_ptr<DisplayBox> pDisplayBox,
                                                      WindowBox& windowBox) noexcept {
    Display* display = pDisplayBox->getDisplay();
    const int width = windowBox.getWidth();
    const int height = windowBox.getHeight();

    auto shmInfo = std::make_unique<XShmSegmentInfo>();
    XImage* image =
        XShmCreateImage(display, nullptr, windowBox.getPlanes(), ZPixmap, nullptr, shmInfo.get(), width, height);
    if (image == nullptr) {
        return std::unexpected{Error{ECate::eX11, 0}};
    }

    shmInfo->shmid = shmget(IPC_PRIVATE, image->bytes_per_line * image->height, IPC_CREAT | 0777);
    if (shmInfo->shmid == -1) {
        return std::unexpected{Error{ECate::eSys, errno}};
    }

    void* pData = shmat(shmInfo->shmid, nullptr, 0);
    if (pData == (void*)-1) {
        return std::unexpected{Error{ECate::eSys, errno}};
    }
    image->data = (char*)pData;
    shmInfo->shmaddr = image->data;
    shmInfo->readOnly = False;

    Status status = XShmAttach(display, shmInfo.get());
    if (status == 0) {
        return std::unexpected{Error{ECate::eX11, 0}};
    }

    return ImageShmBox{std::move(pDisplayBox), image, std::move(shmInfo)};
}

std::expected<void, Error> ImageShmBox::fetch(WindowBox& windowBox) noexcept {
    Display* display = pDisplayBox_->getDisplay();
    Window window = windowBox.getWindow();

    Status status = XShmGetImage(display, window, image_, 0, 0, AllPlanes);
    if (status == 0) {
        return std::unexpected{Error{ECate::eX11, 0}};
    }
    return {};
}

void ImageShmBox::copyTo(std::byte* pData) const noexcept {
    const int bufferSize = image_->width * image_->height * (image_->bits_per_pixel / 8);
    std::memcpy(pData, image_->data, bufferSize);
}

}  // namespace tcap::x11
