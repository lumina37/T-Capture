#include <fstream>
#include <print>
#include <thread>

#include "../../sample_helper.hpp"
#include "tcap/tcap.hpp"

struct Future {
    struct promise_type {
        static constexpr Future get_return_object() { return {}; }
        static constexpr std::suspend_never initial_suspend() noexcept { return {}; }
        static constexpr std::suspend_always final_suspend() noexcept { return {}; }
        static constexpr void return_void() noexcept {}
        static constexpr void unhandled_exception() {}
    };
};

Future sampleOneFrame(tcap::mf::ReaderAsyncBox& readerBox, std::vector<std::byte>& frameData) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        auto sampleBoxRes = co_await readerBox.sample();
        if (!sampleBoxRes) continue;

        auto& sampleBox = sampleBoxRes.value();
        if (sampleBox.getPSample() == nullptr) continue;

        auto bufferBox = tcap::mf::BufferBox::create(sampleBox) | unwrap;
        bufferBox.copyTo(frameData.data()) | unwrap;
        break;
    }

    std::ofstream outFStream{"async.yuv"};
    outFStream.write((char*)frameData.data(), (std::streamsize)frameData.size());
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
    auto reader = tcap::mf::ReaderAsyncBox::create(sourceBox) | unwrap;

    auto readerTypeBox = tcap::mf::ReaderTypeBox::create(reader.getReaderBox()) | unwrap;
    const auto& mediaTypeBox = readerTypeBox.getCurrentMediaTypeBox();
    std::println("subType={}", mediaTypeBox.getSubTypeFourCC().strView());
    std::println("fps={}", mediaTypeBox.getApproxFps());
    std::println("width={}, height={}", mediaTypeBox.getWidth(), mediaTypeBox.getHeight());

    std::vector<std::byte> frameData(mediaTypeBox.getWidth() * mediaTypeBox.getHeight() / 2 * 3);

    sampleOneFrame(reader, frameData);
    std::this_thread::sleep_for(std::chrono::seconds(3));  // Wait until the coroutine is done

    tcap::mf::globalDestroy();
}
