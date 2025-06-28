#include <fstream>
#include <print>
#include <tcap/screen/x11/image/shared.hpp>
#include <thread>
#include <vector>

#include "../../sample_helper.hpp"
#include "tcap.hpp"

int main() {
    auto pDisplayBox = std::make_shared<tcap::x11::DisplayBox>(tcap::x11::DisplayBox::create() | unwrap);
    tcap::x11::DisplayCaps displayCaps = tcap::x11::DisplayCaps::create(*pDisplayBox) | unwrap;
    if (!displayCaps.supportImageShm()) {
        std::println("do not support ImageShm");
        return -1;
    }

    auto screenBoxes = tcap::x11::ScreenBox::createBoxes(*pDisplayBox) | unwrap;
    auto& screenBox = screenBoxes[0];
    tcap::x11::TopWindowBoxes windowsBoxes = tcap::x11::TopWindowBoxes::create(*pDisplayBox, screenBoxes[0]) | unwrap;
    tcap::x11::WindowBox windowBox = tcap::x11::WindowBox::createRoot(*pDisplayBox, screenBox) | unwrap;
    std::println("root window size={}x{}, planes={}", windowBox.getWidth(), windowBox.getHeight(),
                 windowBox.getPlanes());

    tcap::x11::ImageShmBox imageBox = tcap::x11::ImageShmBox::create(pDisplayBox, windowBox) | unwrap;
    imageBox.fetch(windowBox) | unwrap;

    std::vector<std::byte> imageData(imageBox.getWidth() * imageBox.getHeight() * imageBox.getBitPerPixel() / 8);
    imageBox.copyTo(imageData.data());

    std::ofstream outFStream{"image.bin"};
    outFStream.write((char*)imageData.data(), (std::streamsize)imageData.size());
    outFStream.close();
}
