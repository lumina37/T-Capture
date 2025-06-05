#include "sample_helper.hpp"
#include "tcap.hpp"

int main() {
    tcap::mf::globalInit() | unwrap;

    tcap::mf::DeviceSet deviceSet = tcap::mf::DeviceSet::create() | unwrap;
    for (const auto& pDevice : deviceSet.getPDeviceBoxes()) {
        auto source = tcap::mf::SourceBox::create(pDevice) | unwrap;
        std::println("Device name: {}", pDevice->getName());
        std::println("Device ptr: 0x{:x}", (size_t)pDevice->getPDevice());
    }

    tcap::mf::globalDestroy();
}
