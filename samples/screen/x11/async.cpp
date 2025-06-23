#include <fstream>
#include <print>
#include <thread>

#include "../../sample_helper.hpp"
#include "tcap.hpp"

int main() {
    tcap::x11::DisplayBox displayBox = tcap::x11::DisplayBox::create() | unwrap;
    auto screenBoxes = tcap::x11::ScreenBox::createBoxes(displayBox) | unwrap;
    tcap::x11::WindowBoxes windowsBoxes = tcap::x11::WindowBoxes::create(displayBox, screenBoxes[0]) | unwrap;
}
