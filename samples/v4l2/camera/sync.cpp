#include "../../sample_helper.hpp"
#include "tcap.hpp"

int main() {
    tcap::v4l2::DevicePaths devicePaths = tcap::v4l2::DevicePaths::create() | unwrap;
    tcap::v4l2::DeviceBox deviceBox = tcap::v4l2::DeviceBox::create(devicePaths.getPath(0)) | unwrap;
    tcap::v4l2::FormatBoxes formatBoxes = tcap::v4l2::FormatBoxes::create(deviceBox) | unwrap;
    if (devicePaths.empty()) return 1;
}
