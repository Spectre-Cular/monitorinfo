#include "monitor_info.h"
#include <wingdi.h>
#include <winuser.h>
#include <algorithm>

#ifdef _WIN64
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#endif

using namespace godot;

const std::vector<Vector2i> commonResolutions = {
    {1920, 1080},
    {1366, 768},
    {1280, 1024},
    {1440, 900},
    {1600, 900},
    {1680, 1050},
    {1280, 800},
    {1024, 768},
    {2560, 1440},
    {3840, 2160},
    {3440, 1440}
};

CMonitorInfo::CMonitorInfo() {}

bool CMonitorInfo::IsCommonResolution(int32_t width, int32_t height)
{
    for (const auto& res : commonResolutions)
        if (res.x == width && res.y == height)
            return true;

    return false;
}

void CMonitorInfo::GetMonitorResolutions(const std::wstring& deviceName, std::vector<TScreenResolution>& resolutions)
{
    DEVMODE devMode = {};
    devMode.dmSize = sizeof(devMode);
    for (int iModeNum = 0; EnumDisplaySettings(deviceName.c_str(), iModeNum, &devMode); ++iModeNum)
    {
        if (IsCommonResolution(devMode.dmPelsWidth, devMode.dmPelsHeight)) {
            TScreenResolution res = { 
                Vector2i(int32_t(devMode.dmPelsWidth), int32_t(devMode.dmPelsHeight)), 
                int(devMode.dmDisplayFrequency) 
            };

            if (std::find_if(resolutions.begin(), resolutions.end(), [&res](const TScreenResolution& r) {
                return r.Dimensions == res.Dimensions && r.RefreshRate == res.RefreshRate;
            }) == resolutions.end()) {
                resolutions.push_back(res);
            }
        }
    }
}

std::vector<CMonitor> CMonitorInfo::Update()
{
    monitorInfo.clear();

    std::vector<DISPLAYCONFIG_PATH_INFO> paths;
    std::vector<DISPLAYCONFIG_MODE_INFO> modes;
    uint32_t flags = QDC_ONLY_ACTIVE_PATHS | QDC_VIRTUAL_MODE_AWARE;
    int64_t result = ERROR_SUCCESS;

    do
    {
        uint32_t pathCount, modeCount;
        result = GetDisplayConfigBufferSizes(flags, &pathCount, &modeCount);

        if (result != ERROR_SUCCESS)
            return monitorInfo;

        paths.resize(pathCount);
        modes.resize(modeCount);

        result = QueryDisplayConfig(flags, &pathCount, paths.data(), &modeCount, modes.data(), nullptr);

        paths.resize(pathCount);
        modes.resize(modeCount);

    } while (result == ERROR_INSUFFICIENT_BUFFER);

    if (result != ERROR_SUCCESS)
        return monitorInfo;

    std::vector<std::wstring> monitorNames;
    for (auto& path : paths)
    {
        DISPLAYCONFIG_TARGET_DEVICE_NAME targetName = {};
        targetName.header.adapterId = path.targetInfo.adapterId;
        targetName.header.id = path.targetInfo.id;
        targetName.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_TARGET_NAME;
        targetName.header.size = sizeof(targetName);
        result = DisplayConfigGetDeviceInfo(&targetName.header);

        if (result != ERROR_SUCCESS)
            continue;

        auto fallbackName = std::wstring(L"Display #") + std::to_wstring(monitorNames.size() + 1);
        monitorNames.push_back(targetName.flags.friendlyNameFromEdid ? targetName.monitorFriendlyDeviceName : fallbackName);
    }

    int monitorNameIndex = 0;
    for (int deviceIndex = 0;; ++deviceIndex)
    {
        DISPLAY_DEVICE displayDevice = {};
        displayDevice.cb = sizeof(displayDevice);

        if (!EnumDisplayDevices(nullptr, deviceIndex, &displayDevice, 0))
            break;

        if (!(displayDevice.StateFlags & DISPLAY_DEVICE_ACTIVE))
            continue;

        CMonitor monitor;
        monitor.DeviceName = displayDevice.DeviceName;
        std::vector<TScreenResolution> resolutions;
        GetMonitorResolutions(monitor.DeviceName, resolutions);
        std::sort(resolutions.begin(), resolutions.end(), [](const TScreenResolution& a, const TScreenResolution& b) {
            return (a.Dimensions.x * a.Dimensions.y) > (b.Dimensions.x * b.Dimensions.y) ||
            ((a.Dimensions.x * a.Dimensions.y) == (b.Dimensions.x * b.Dimensions.y) && a.RefreshRate > b.RefreshRate);
        });

        monitor.FriendlyName = monitorNames[monitorNameIndex];
        monitor.IsPrimary = displayDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE;
        if (!resolutions.empty()) {
            monitor.Resolutions = resolutions;
        }
        monitorInfo.push_back(monitor);
        ++monitorNameIndex;
    }

    return monitorInfo;
}
