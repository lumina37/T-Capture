#include <fstream>
#include <thread>

#include "sample_helper.hpp"
#include "tcap.hpp"
#include "tcap/helper/charset.hpp"

struct Future {
    struct promise_type {
        Future get_return_object() { return {}; }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() {}
    };
};

Future sampleOneFrame(tcap::mf::AsyncReaderBox& readerBox, std::vector<std::byte>& frameData) {
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
    auto readerBox = tcap::mf::AsyncReaderBox::create(sourceBox) | unwrap;

    auto readerTypeBox = tcap::mf::ReaderTypeBox::create(readerBox) | unwrap;
    const auto& mediaTypeBox = readerTypeBox.getCurrentMediaTypeBox();
    std::println("subType={}", (int)mediaTypeBox.getSubType());
    std::println("fps={}", mediaTypeBox.getApproxFps());
    std::println("width={}, height={}", mediaTypeBox.getWidth(), mediaTypeBox.getHeight());

    std::vector<std::byte> frameData(mediaTypeBox.getWidth() * mediaTypeBox.getHeight() / 2 * 3);

    sampleOneFrame(readerBox, frameData);
    std::this_thread::sleep_for(std::chrono::seconds(3));  // Wait until the coroutine is done

    tcap::mf::globalDestroy();
}
