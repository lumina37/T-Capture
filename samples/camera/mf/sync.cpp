#include <fstream>
#include <print>
#include <thread>

#include <mfapi.h>

#include "../../sample_helper.hpp"
#include "tcap/tcap.hpp"

int main() {
    tcap::mf::globalInit() | unwrap;

    tcap::mf::DeviceBoxes deviceBoxes = tcap::mf::DeviceBoxes::create() | unwrap;
    if (deviceBoxes.empty()) return 1;

    for (const auto& pDeviceBox : deviceBoxes.getPDeviceBoxes()) {
        std::println("Device name: {}", pDeviceBox->getName());
    }

    auto sourceBox = tcap::mf::SourceBox::create(deviceBoxes.getPDeviceBox(0)) | unwrap;
    auto reader = tcap::mf::ReaderSync::create(sourceBox) | unwrap;

    auto readerTypeBox = tcap::mf::ReaderTypeBox::create(reader.getReaderBox()) | unwrap;
    const auto& oldMediaTypeBox = readerTypeBox.getCurrentMediaTypeBox();
    std::println("subType={}", oldMediaTypeBox.getSubTypeFourCC().strView());
    std::println("width={}, height={}", oldMediaTypeBox.getWidth(), oldMediaTypeBox.getHeight());

    auto* pMediaTypeBox = &oldMediaTypeBox;
    for (auto& mTypeBox : readerTypeBox.getNativeMediaTypeBoxes()) {
        if (mTypeBox.getSubType() == MFVideoFormat_NV12) pMediaTypeBox = &mTypeBox;
    }
    reader.setMediaType(*pMediaTypeBox) | unwrap;
    std::println("After switching");
    std::println("subType={}", pMediaTypeBox->getSubTypeFourCC().strView());
    std::println("width={}, height={}", pMediaTypeBox->getWidth(), pMediaTypeBox->getHeight());

    std::vector<std::byte> frameData(pMediaTypeBox->getWidth() * pMediaTypeBox->getHeight() / 2 * 3);
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
