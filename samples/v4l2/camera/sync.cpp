#include "../../sample_helper.hpp"
#include "tcap.hpp"

int main() {
    tcap::v4l2::DevicePaths devicePaths = tcap::v4l2::DevicePaths::create() | unwrap;
    if (devicePaths.empty()) return 1;
}
