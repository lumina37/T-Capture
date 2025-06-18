#include <fstream>
#include <print>
#include <thread>

#include <catch2/catch_test_macros.hpp>

#include "../../sample_helper.hpp"
#include "tcap.hpp"
#include "tcap/helper/fourcc.hpp"

TEST_CASE("Camera capture", "v4l2::camera::sync") {
    tcap::v4l2::DevicePaths devicePaths = tcap::v4l2::DevicePaths::create() | unwrap;
    if (devicePaths.empty()) return;

    auto deviceBox = tcap::v4l2::DeviceBox::create(devicePaths.getPath(0)) | unwrap;

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
                std::println("nativeFormat={}, size={}x{}, fps={}", tcap::FourCC(nativeFormat).strView(), width, height,
                             fpsBox.approxFps());
            }
        }
    }

    // Active format
    tcap::v4l2::ActiveFormatBox activeFormatBox = tcap::v4l2::ActiveFormatBox::create(deviceBox) | unwrap;
    std::println("default: format={}, size={}x{}", tcap::FourCC(activeFormatBox.getFormat()).strView(),
                 activeFormatBox.getWidth(), activeFormatBox.getHeight());

    activeFormatBox.setWidth(640);
    activeFormatBox.setHeight(480);

    activeFormatBox.apply(deviceBox) | unwrap;

    activeFormatBox = tcap::v4l2::ActiveFormatBox::create(deviceBox) | unwrap;
    std::println("new: format={}, size={}x{}", tcap::FourCC(activeFormatBox.getFormat()).strView(),
                 activeFormatBox.getWidth(), activeFormatBox.getHeight());

    // Queue
    tcap::v4l2::QueueCaps bufferCapsBox = tcap::v4l2::QueueCaps::create(deviceBox) | unwrap;
    std::println("support mmap cache hints: {}", bufferCapsBox.supportMMapCacheHints());
    std::println("support DMA buffer: {}", bufferCapsBox.supportDMABuf());
    tcap::v4l2::QueueMMapBox queueBox = tcap::v4l2::QueueMMapBox::create(std::move(deviceBox)) | unwrap;
    queueBox.turnOnStream() | unwrap;

    std::vector<std::byte> frameData(activeFormatBox.getWidth() * activeFormatBox.getHeight() * 2);
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        auto sampleRes = queueBox.popBuffer();
        if (!sampleRes && sampleRes.error().code == EAGAIN) continue;
        auto sample = std::move(sampleRes) | unwrap;
        std::println("sample at timestamp={}", sample.getTimestampNs());
        sample.copyTo(frameData.data()) | unwrap;
        queueBox.pushBuffer(sample.take()) | unwrap;
        break;
    }

    std::ofstream outFStream{"sync.yuv"};
    outFStream.write((char*)frameData.data(), frameData.size());
    outFStream.close();
}
