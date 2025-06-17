#include <print>

#include "../../sample_helper.hpp"
#include "tcap.hpp"
#include "tcap/camera/v4l2/format/active.hpp"

int main() {
    tcap::v4l2::DevicePaths devicePaths = tcap::v4l2::DevicePaths::create() | unwrap;
    if (devicePaths.empty()) return 1;

    tcap::v4l2::DeviceBox deviceBox = tcap::v4l2::DeviceBox::create(devicePaths.getPath(0)) | unwrap;

    // Native formats
    const auto& nativeFormatBoxes = tcap::v4l2::NativeFormatBox::createBoxes(deviceBox) | unwrap;
    for (const auto& nativeFormatBox : nativeFormatBoxes) {
        const uint32_t nativeFormat = nativeFormatBox.getFormat();
        const auto& resolutionBoxes = tcap::v4l2::ResolutionBox::createBoxes(deviceBox, nativeFormat) | unwrap;
        for (const auto& resolutionBox : resolutionBoxes) {
            const uint32_t width = resolutionBox.getWidth();
            const uint32_t height = resolutionBox.getHeight();
            const auto& fpsBoxes = tcap::v4l2::FpsBox::createBoxes(deviceBox, nativeFormat, width, height) | unwrap;
            for (const auto& fpsBox : fpsBoxes) {
                std::println("nativeFormat={}, size={}x{}, fps={}", nativeFormat, width, height, fpsBox.approxFps());
            }
        }
    }

    // Active format
    tcap::v4l2::ActiveFormatBox activeFormatBox = tcap::v4l2::ActiveFormatBox::create(deviceBox) | unwrap;
    std::println("default: format={}, size={}x{}", activeFormatBox.getFormat(), activeFormatBox.getWidth(),
                 activeFormatBox.getHeight());

    activeFormatBox.setWidth(640);
    activeFormatBox.setHeight(480);

    activeFormatBox.apply(deviceBox) | unwrap;

    activeFormatBox = tcap::v4l2::ActiveFormatBox::create(deviceBox) | unwrap;
    std::println("new: format={}, size={}x{}", activeFormatBox.getFormat(), activeFormatBox.getWidth(),
                 activeFormatBox.getHeight());

    // Queue
    tcap::v4l2::QueueCapsBox bufferCapsBox = tcap::v4l2::QueueCapsBox::create(deviceBox) | unwrap;
    std::println("support mmap cache hints: {}", bufferCapsBox.supportMMapCacheHints());
    std::println("support DMA buffer: {}", bufferCapsBox.supportDMABuf());


}
