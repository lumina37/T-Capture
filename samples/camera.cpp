#include "sample_helper.hpp"
#include "tcap.hpp"

int main() {
    tcap::mf::globalInit() | unwrap;

    tcap::mf::DeviceSet deviceSet = tcap::mf::DeviceSet::create() | unwrap;
    for (auto& device : deviceSet.getDevices()) {
        std::println("Device name: {}", device.getName());
    }

    tcap::mf::globalDestroy();
}
