#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <expected>
#include <utility>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>

#include "tcap/screen/x11/display/box.hpp"
#include "tcap/screen/x11/image/shared.hpp"
#include "tcap/screen/x11/window/box.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::x11 {

ImageShmBox::ImageShmBox(std::shared_ptr<DisplayBox>&& pDisplayBox, XImage* pImage,
                         std::unique_ptr<XShmSegmentInfo>&& shmInfo) noexcept
    : pDisplayBox_(std::move(pDisplayBox)), pImage_(pImage), shmInfo_(std::move(shmInfo)) {}

ImageShmBox::ImageShmBox(ImageShmBox&& rhs) noexcept
    : pDisplayBox_(std::move(rhs.pDisplayBox_)),
      pImage_(std::exchange(rhs.pImage_, nullptr)),
      shmInfo_(std::move(rhs.shmInfo_)) {}

ImageShmBox& ImageShmBox::operator=(ImageShmBox&& rhs) noexcept {
    pDisplayBox_ = std::move(rhs.pDisplayBox_);
    pImage_ = std::exchange(rhs.pImage_, nullptr);
    shmInfo_ = std::move(rhs.shmInfo_);
    return *this;
}

ImageShmBox::~ImageShmBox() noexcept {
    if (pImage_ == nullptr) return;
    Display* pDisplay = pDisplayBox_->getPDisplay();
    XShmDetach(pDisplay, shmInfo_.get());
    XDestroyImage(pImage_);
    shmdt(shmInfo_->shmaddr);
    shmctl(shmInfo_->shmid, IPC_RMID, nullptr);
    pImage_ = nullptr;
}

std::expected<ImageShmBox, Error> ImageShmBox::create(std::shared_ptr<DisplayBox> pDisplayBox,
                                                      WindowBox& windowBox) noexcept {
    Display* pDisplay = pDisplayBox->getPDisplay();
    const int width = windowBox.getWidth();
    const int height = windowBox.getHeight();

    auto shmInfo = std::make_unique<XShmSegmentInfo>();
    XImage* pImage =
        XShmCreateImage(pDisplay, nullptr, windowBox.getPlanes(), ZPixmap, nullptr, shmInfo.get(), width, height);
    if (pImage == nullptr) {
        return std::unexpected{Error{ECate::eX11, 0, "XShmCreateImage failed"}};
    }

    shmInfo->shmid = shmget(IPC_PRIVATE, pImage->bytes_per_line * pImage->height, IPC_CREAT | 0777);
    if (shmInfo->shmid == -1) {
        return std::unexpected{Error{ECate::eSys, errno}};
    }

    void* pData = shmat(shmInfo->shmid, nullptr, 0);
    if (pData == (void*)-1) {
        return std::unexpected{Error{ECate::eSys, errno}};
    }
    pImage->data = (char*)pData;
    shmInfo->shmaddr = pImage->data;
    shmInfo->readOnly = False;

    Status status = XShmAttach(pDisplay, shmInfo.get());
    if (status == 0) {
        return std::unexpected{Error{ECate::eX11, 0, "XShmAttach failed"}};
    }

    return ImageShmBox{std::move(pDisplayBox), pImage, std::move(shmInfo)};
}

std::expected<void, Error> ImageShmBox::fetch(WindowBox& windowBox) noexcept {
    Display* pDisplay = pDisplayBox_->getPDisplay();
    Window window = windowBox.getWindow();

    Status status = XShmGetImage(pDisplay, window, pImage_, 0, 0, AllPlanes);
    if (status == 0) {
        return std::unexpected{Error{ECate::eX11, -1}};
    }
    return {};
}

void ImageShmBox::copyTo(std::byte* pData) const noexcept {
    const int bufferSize = pImage_->width * pImage_->height * (pImage_->bits_per_pixel / 8);
    std::memcpy(pData, pImage_->data, bufferSize);
}

}  // namespace tcap::x11
