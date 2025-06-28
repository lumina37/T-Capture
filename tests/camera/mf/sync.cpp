#include <fstream>
#include <thread>

#include <catch2/catch_test_macros.hpp>

#include "../../sample_helper.hpp"
#include "tcap.hpp"

TEST_CASE("Camera capture", "camera::mf::sync") {
    tcap::mf::globalInit() | unwrap;

    tcap::mf::DeviceBoxes deviceBoxes = tcap::mf::DeviceBoxes::create() | unwrap;
    if (deviceBoxes.empty()) return;

    for (const auto& pDeviceBox : deviceBoxes.getPDeviceBoxes()) {
        std::println("Device name: {}", pDeviceBox->getName());
    }

    auto sourceBox = tcap::mf::SourceBox::create(deviceBoxes.getPDeviceBox(0)) | unwrap;
    auto reader = tcap::mf::ReaderSync::create(sourceBox) | unwrap;

    auto readerTypeBox = tcap::mf::ReaderTypeBox::create(reader.getReaderBox()) | unwrap;
    const auto& oldMediaTypeBox = readerTypeBox.getCurrentMediaTypeBox();
    std::println("width={}, height={}", oldMediaTypeBox.getWidth(), oldMediaTypeBox.getHeight());

    const auto& mediaTypeBox = readerTypeBox.getNativeMediaTypeBoxes().back();
    reader.setMediaType(mediaTypeBox) | unwrap;
    std::println("After switching");
    std::println("subType={}", mediaTypeBox.getSubTypeFourCC().strView());
    std::println("width={}, height={}", mediaTypeBox.getWidth(), mediaTypeBox.getHeight());

    std::vector<std::byte> frameData(mediaTypeBox.getWidth() * mediaTypeBox.getHeight() / 2 * 3);
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        auto sampleBoxRes = reader.sample();
        if (!sampleBoxRes) continue;

        auto sampleBox = std::move(sampleBoxRes.value());
        if (sampleBox.getPSample() == nullptr) continue;

        auto bufferBox = tcap::mf::BufferBox::create(sampleBox) | unwrap;
        bufferBox.copyTo(frameData.data()) | unwrap;
        break;
    }

    std::ofstream outFStream{"sync.yuv"};
    outFStream.write((char*)frameData.data(), frameData.size());
    outFStream.close();

    tcap::mf::globalDestroy();
}
