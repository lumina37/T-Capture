#include <fstream>
#include <print>
#include <thread>

#include "../../sample_helper.hpp"
#include "tcap.hpp"

int main() {
    tcap::v4l2::DevicePaths devicePaths = tcap::v4l2::DevicePaths::create() | unwrap;
    if (devicePaths.empty()) return 1;

    auto deviceBox = tcap::v4l2::DeviceBox::create(devicePaths.getPath(0)) | unwrap;

    // Native formats
    const auto& formatNativeBoxes = tcap::v4l2::FormatNativeBox::createBoxes(deviceBox) | unwrap;
    for (const auto& formatNativeBox : formatNativeBoxes) {
        const uint32_t nativeFormat = formatNativeBox.getFormat();
        const auto& resolutionBoxes = tcap::v4l2::ResolutionBox::createBoxes(deviceBox, nativeFormat) | unwrap;
        for (const auto& resolutionBox : resolutionBoxes) {
            const uint32_t width = resolutionBox.getWidth();
            const uint32_t height = resolutionBox.getHeight();
            const auto& fpsBoxes = tcap::v4l2::FpsBox::createBoxes(deviceBox, nativeFormat, width, height) | unwrap;
            for (const auto& fpsBox : fpsBoxes) {
                std::println("nativeFormat={}, size={}x{}, fps={}", tcap::FourCC(nativeFormat).strView(), width, height,
                             fpsBox.approxFps());
            }
        }
    }

    // Active format
    tcap::v4l2::FormatActiveBox formatActiveBox = tcap::v4l2::FormatActiveBox::create(deviceBox) | unwrap;
    std::println("default: format={}, size={}x{}", tcap::FourCC(formatActiveBox.getFormat()).strView(),
                 formatActiveBox.getWidth(), formatActiveBox.getHeight());

    formatActiveBox.setWidth(640);
    formatActiveBox.setHeight(480);

    formatActiveBox.apply(deviceBox) | unwrap;

    formatActiveBox = tcap::v4l2::FormatActiveBox::create(deviceBox) | unwrap;
    std::println("new: format={}, size={}x{}", tcap::FourCC(formatActiveBox.getFormat()).strView(),
                 formatActiveBox.getWidth(), formatActiveBox.getHeight());

    // Queue
    tcap::v4l2::QueueCaps bufferCapsBox = tcap::v4l2::QueueCaps::create(deviceBox) | unwrap;
    std::println("support mmap cache hints: {}", bufferCapsBox.supportMMapCacheHints());
    std::println("support DMA buffer: {}", bufferCapsBox.supportDMABuf());
    tcap::v4l2::QueueMMapBox queueBox = tcap::v4l2::QueueMMapBox::create(std::move(deviceBox)) | unwrap;
    queueBox.turnOnStream() | unwrap;

    std::vector<std::byte> frameData(formatActiveBox.getWidth() * formatActiveBox.getHeight() * 2);
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        auto sampleRes = queueBox.popBuffer();
        if (!sampleRes && sampleRes.error().code == EAGAIN) continue;
        auto sample = std::move(sampleRes) | unwrap;
        std::println("sample at timestamp={}", sample.getTimestampMs());
        sample.copyTo(frameData.data()) | unwrap;
        queueBox.pushBuffer(sample.take()) | unwrap;
        break;
    }

    std::ofstream outFStream{"sync.yuv"};
    outFStream.write((char*)frameData.data(), (std::streamsize)frameData.size());
    outFStream.close();
}
