#pragma once

#include <godot_cpp/variant/vector2i.hpp>
#include <vector>
#include <string>

#ifdef _WIN64
#include <basetsd.h>
#include <windows.h>
#endif

struct TScreenResolution {
    godot::Vector2i Dimensions;
    int RefreshRate;
};

struct CMonitor {
    std::wstring FriendlyName;
    std::wstring DeviceName;
    bool IsPrimary;
    std::vector<TScreenResolution> Resolutions;
};

class CMonitorInfo
{
public:
    CMonitorInfo();
    std::vector<CMonitor> Update();

private:
    std::vector<CMonitor> monitorInfo;

    bool IsCommonResolution(int32_t width, int32_t height);
    void GetMonitorResolutions(const std::wstring& deviceName, std::vector<TScreenResolution>& resolutions);
};
