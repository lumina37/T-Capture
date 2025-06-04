#include <print>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <mfapi.h>
#include <mfidl.h>
#include <mfobjects.h>
#include <mfreadwrite.h>
#include <iostream>
#include <string>

#ifndef _TLCT_LIB_HEADER_ONLY
#    include "tcap/camera/windows/impl.hpp"
#endif

namespace tcap {

void EnumerateCaptureDevices() {
    IMFAttributes* pAttributes = nullptr;
    IMFActivate** ppDevices = nullptr;
    UINT32 count = 0;

    HRESULT hr = MFCreateAttributes(&pAttributes, 1);
    if (FAILED(hr)) {
        std::cerr << "MFCreateAttributes failed: " << std::hex << hr << std::endl;
        return;
    }

    hr = pAttributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                              MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);  // 视频捕捉设备

    if (FAILED(hr)) {
        std::cerr << "SetGUID failed: " << std::hex << hr << std::endl;
        pAttributes->Release();
        return;
    }

    hr = MFEnumDeviceSources(pAttributes, &ppDevices, &count);
    if (FAILED(hr)) {
        std::cerr << "MFEnumDeviceSources failed: " << std::hex << hr << std::endl;
        pAttributes->Release();
        return;
    }

    std::cout << "Discover " << count << " devices:" << std::endl;

    for (UINT32 i = 0; i < count; i++) {
        WCHAR* friendlyName = nullptr;
        UINT32 nameLength = 0;

        hr = ppDevices[i]->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &friendlyName, &nameLength);

        if (SUCCEEDED(hr)) {
            std::wcout << "Device " << i << ": " << friendlyName << std::endl;
            CoTaskMemFree(friendlyName);
        }

        ppDevices[i]->Release();
    }

    CoTaskMemFree(ppDevices);
    pAttributes->Release();
}

}  // namespace tcap
