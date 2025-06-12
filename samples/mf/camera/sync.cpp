#include <fstream>
#include <thread>

#include "../../sample_helper.hpp"
#include "tcap.hpp"

int main() {
    tcap::mf::globalInit() | unwrap;

    tcap::mf::DeviceBoxes deviceBoxes = tcap::mf::DeviceBoxes::create() | unwrap;
    if (deviceBoxes.empty()) return 1;

    for (const auto& pDeviceBox : deviceBoxes.getPDeviceBoxes()) {
        std::println("Device name: {}", pDeviceBox->getName());
    }

    auto sourceBox = tcap::mf::SourceBox::create(deviceBoxes.getPDeviceBox(0)) | unwrap;
    auto readerBox = tcap::mf::ReaderSyncBox::create(sourceBox) | unwrap;

    auto readerTypeBox = tcap::mf::ReaderTypeBox::create(readerBox) | unwrap;
    const auto& oldMediaTypeBox = readerTypeBox.getCurrentMediaTypeBox();
    std::println("width={}, height={}", oldMediaTypeBox.getWidth(), oldMediaTypeBox.getHeight());

    const auto& mediaTypeBox = readerTypeBox.getNativeMediaTypeBoxes().back();
    readerBox.setMediaType(mediaTypeBox) | unwrap;
    std::println("After switching", (int)mediaTypeBox.getSubType());
    std::println("width={}, height={}", mediaTypeBox.getWidth(), mediaTypeBox.getHeight());

    std::vector<std::byte> frameData(mediaTypeBox.getWidth() * mediaTypeBox.getHeight() / 2 * 3);
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        auto sampleBoxRes = readerBox.sample();
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
