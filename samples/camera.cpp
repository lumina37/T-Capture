#include "sample_helper.hpp"
#include "tcap.hpp"
#include "tcap/helper/charset.hpp"

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
    for (auto& mediaTypeBox : readerTypeBox.getStreamNativeMediaTypeBoxes(0)) {
        std::println("sub type={}", (int)mediaTypeBox.getSubType());
        std::println("fps={}", mediaTypeBox.getApproxFps());
        std::println("width={}, height={}", mediaTypeBox.getWidth(), mediaTypeBox.getHeight());
    }

    tcap::mf::globalDestroy();
}
