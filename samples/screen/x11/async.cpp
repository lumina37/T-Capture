#include <fstream>
#include <print>
#include <thread>
#include <vector>

#include "../../sample_helper.hpp"
#include "tcap.hpp"
#include "tcap/screen/x11/image.hpp"

int main() {
    tcap::x11::DisplayBox displayBox = tcap::x11::DisplayBox::create() | unwrap;
    auto screenBoxes = tcap::x11::ScreenBox::createBoxes(displayBox) | unwrap;
    auto& screenBox = screenBoxes[0];
    tcap::x11::TopWindowBoxes windowsBoxes = tcap::x11::TopWindowBoxes::create(displayBox, screenBoxes[0]) | unwrap;
    tcap::x11::WindowBox windowBox = tcap::x11::WindowBox::createRoot(displayBox, screenBox) | unwrap;
    std::println("root window size={}x{}, planes={}", windowBox.getWidth(), windowBox.getHeight(),
                 windowBox.getPlanes());

    tcap::x11::ImageBox imageBox = tcap::x11::ImageBox::create(displayBox, windowBox) | unwrap;

    std::vector<std::byte> imageData(imageBox.getWidth() * imageBox.getHeight() * imageBox.getBitPerPixel() / 8);
    imageBox.copyTo(imageData.data());

    std::ofstream outFStream{"image.bin"};
    outFStream.write((char*)imageData.data(), (std::streamsize)imageData.size());
    outFStream.close();
}
