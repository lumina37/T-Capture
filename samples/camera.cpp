#include <print>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <mfapi.h>
#include <mferror.h>
#include <mfidl.h>
#include <mfobjects.h>
#include <mfreadwrite.h>
#include <iostream>
#include <string>

#include "tcap.hpp"

int main() {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        std::cerr << "CoInitializeEx failed: " << std::hex << hr << std::endl;
        return -1;
    }
    hr = MFStartup(MF_VERSION);
    if (FAILED(hr)) {
        std::cerr << "MFStartup failed: " << std::hex << hr << std::endl;
        CoUninitialize();
        return -1;
    }

    tcap::EnumerateCaptureDevices();

    MFShutdown();
    CoUninitialize();
}
