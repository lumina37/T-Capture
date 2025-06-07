#include <fstream>
#include <thread>

#include <mfobjects.h>

#include "sample_helper.hpp"
#include "tcap.hpp"
#include "tcap/helper/charset.hpp"

struct SimpleTask {
    struct promise_type {
        SimpleTask get_return_object() { return {}; }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
};

SimpleTask SampleOneFrame(std::vector<std::byte>& frameData, tcap::mf::ReaderBox& readerBox) {
    auto sampleBoxRes = co_await readerBox.sample();

    auto& sampleBox = sampleBoxRes.value();
    auto bufferBox = tcap::mf::BufferBox::create(sampleBox) | unwrap;
    bufferBox.copyTo(frameData.data()) | unwrap;

    std::ofstream outFStream{"out.yuv"};
    outFStream.write((char*)frameData.data(), frameData.size());
    outFStream.close();
}

int main() {
    tcap::mf::globalInit() | unwrap;

    tcap::mf::DeviceBoxes deviceBoxes = tcap::mf::DeviceBoxes::create() | unwrap;
    if (deviceBoxes.empty()) return 1;

    for (const auto& pDeviceBox : deviceBoxes.getPDeviceBoxes()) {
        std::println("Device name: {}", pDeviceBox->getName());
    }

    auto sourceBox = tcap::mf::SourceBox::create(deviceBoxes.getPDeviceBox(0)) | unwrap;
    auto readerBox = tcap::mf::ReaderBox::create(sourceBox) | unwrap;

    auto readerTypeBox = tcap::mf::ReaderTypeBox::create(readerBox) | unwrap;
    const auto& mediaTypeBox = readerTypeBox.getCurrentMediaTypeBox();
    std::println("sub type={}", (int)mediaTypeBox.getSubType());
    std::println("fps={}", mediaTypeBox.getApproxFps());
    std::println("width={}, height={}", mediaTypeBox.getWidth(), mediaTypeBox.getHeight());

    std::vector<std::byte> frameData(mediaTypeBox.getWidth() * mediaTypeBox.getHeight() / 2 * 3);
    for (int i = 0; i < 5; i++) {
        SampleOneFrame(frameData, readerBox);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    tcap::mf::globalDestroy();
}
