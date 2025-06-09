#include <fstream>
#include <thread>

#include "sample_helper.hpp"
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

    std::vector<std::byte> frameData(640 * 360 / 2 * 3);

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        auto sampleBoxRes = readerBox.sample();
        if (!sampleBoxRes) continue;
        auto sampleBox = std::move(sampleBoxRes.value());
        auto bufferBox = tcap::mf::BufferBox::create(sampleBox) | unwrap;
        bufferBox.copyTo(frameData.data()) | unwrap;
        break;
    }

    std::ofstream outFStream{"out.yuv"};
    outFStream.write((char*)frameData.data(), frameData.size());
    outFStream.close();

    tcap::mf::globalDestroy();
}
