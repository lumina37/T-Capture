#include <fstream>
#include <thread>

#include <catch2/catch_test_macros.hpp>

#include "../../sample_helper.hpp"
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
    outFStream.write((char*)frameData.data(), frameData.size());
    outFStream.close();
}

TEST_CASE("Camera capture", "mf::camera::async") {
    tcap::mf::globalInit() | unwrap;

    tcap::mf::DeviceBoxes deviceBoxes = tcap::mf::DeviceBoxes::create() | unwrap;
    if (deviceBoxes.empty()) return;

    for (const auto& pDeviceBox : deviceBoxes.getPDeviceBoxes()) {
        std::println("Device name: {}", pDeviceBox->getName());
    }

    auto sourceBox = tcap::mf::SourceBox::create(deviceBoxes.getPDeviceBox(0)) | unwrap;
    auto readerBox = tcap::mf::ReaderAsyncBox::create(sourceBox) | unwrap;

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
