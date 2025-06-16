#include <print>

#include "../../sample_helper.hpp"
#include "tcap.hpp"

int main() {
    tcap::v4l2::DevicePaths devicePaths = tcap::v4l2::DevicePaths::create() | unwrap;
    tcap::v4l2::DeviceBox deviceBox = tcap::v4l2::DeviceBox::create(devicePaths.getPath(0)) | unwrap;
    const auto& nativeFormatBoxes = tcap::v4l2::NativeFormatBox::createBoxes(deviceBox) | unwrap;
    for (const auto& nativeFormatBox : nativeFormatBoxes) {
        const uint32_t nativeFormat = nativeFormatBox.getFormat();
        const auto& resolutionBoxes = tcap::v4l2::ResolutionBox::createBoxes(deviceBox, nativeFormat) | unwrap;
        for (const auto& resolutionBox : resolutionBoxes) {
            const uint32_t width = resolutionBox.getWidth();
            const uint32_t height = resolutionBox.getHeight();
            const auto& fpsBoxes = tcap::v4l2::FpsBox::createBoxes(deviceBox, nativeFormat, width, height) | unwrap;
            for (const auto& fpsBox : fpsBoxes) {
                const float approxFps = (float)fpsBox.getDenominator() / (float)fpsBox.getNumerator();
                std::println("nativeFormat={}, size={}x{}, fps={}", nativeFormat, width, height, approxFps);
            }
        }
    }

    if (devicePaths.empty()) return 1;
}
