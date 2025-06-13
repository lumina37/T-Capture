#include "../../sample_helper.hpp"
#include "tcap.hpp"

int main() {
    tcap::v4l2::DeviceBoxes deviceBoxes = tcap::v4l2::DeviceBoxes::create() | unwrap;
    if (deviceBoxes.empty()) return 1;
}
