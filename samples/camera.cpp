#include "sample_helper.hpp"
#include "tcap.hpp"

int main() {
    tcap::mf::globalInit() | unwrap;

    tcap::mf::DeviceBoxes deviceBoxes = tcap::mf::DeviceBoxes::create() | unwrap;
    if (deviceBoxes.empty()) return 1;

    for (const auto& pDeviceBox : deviceBoxes.getPDeviceBoxes()) {
        std::println("Device name: {}", pDeviceBox->getName());
        std::println("Device ptr: 0x{:x}", (size_t)pDeviceBox->getPDevice());
    }

    auto sourceBox = tcap::mf::SourceBox::create(deviceBoxes.getPDeviceBox(0)) | unwrap;
    auto presentDescBox = tcap::mf::PresentDescBox::create(sourceBox) | unwrap;
    for (auto& streamDesc : presentDescBox.getStreamDescBoxes()) {
        for (auto& mediaTypeBox : streamDesc.getMediaTypeBoxes()) {
            std::println("Fps: {}", mediaTypeBox.getApproxFps());
        }
    }

    tcap::mf::globalDestroy();
}
