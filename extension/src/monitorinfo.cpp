/* monitorinfo.cpp */

#include "monitorinfo.h"
#include <vector>
#include <string>
#include <godot_cpp/variant/utility_functions.hpp>

ScreenResolution::ScreenResolution() {}
ScreenResolution::~ScreenResolution() {}

void ScreenResolution::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_size"), &ScreenResolution::get_size);
	ClassDB::bind_method(D_METHOD("get_refresh_rate"), &ScreenResolution::get_refresh_rate);
	ClassDB::bind_method(D_METHOD("set_size"), &ScreenResolution::set_size);
	ClassDB::bind_method(D_METHOD("set_refresh_rate"), &ScreenResolution::set_refresh_rate);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "size"), "set_size", "get_size");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "refresh_rate"), "set_refresh_rate", "get_refresh_rate");
}

Monitor::Monitor() {}
Monitor::~Monitor() {}

void Monitor::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_friendly_name"), &Monitor::get_friendly_name);
	ClassDB::bind_method(D_METHOD("get_device_name"), &Monitor::get_device_name);
	ClassDB::bind_method(D_METHOD("get_is_primary"), &Monitor::get_is_primary);
	ClassDB::bind_method(D_METHOD("get_resolutions"), &Monitor::get_resolutions);
	ClassDB::bind_method(D_METHOD("set_friendly_name"), &Monitor::set_friendly_name);
	ClassDB::bind_method(D_METHOD("set_device_name"), &Monitor::set_device_name);
	ClassDB::bind_method(D_METHOD("set_is_primary"), &Monitor::set_is_primary);
	ClassDB::bind_method(D_METHOD("set_resolutions"), &Monitor::set_resolutions);
	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "friendly_name"), "set_friendly_name", "get_friendly_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "device_name"), "set_device_name", "get_device_name");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_primary"), "set_is_primary", "get_is_primary");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "resolutions", PROPERTY_HINT_ARRAY_TYPE, "ScreenResolution", PROPERTY_USAGE_DEFAULT), "set_resolutions", "get_resolutions");
}

MonitorInfo::MonitorInfo() {}
MonitorInfo::~MonitorInfo() {}

Array MonitorInfo::get_info() {
    auto monitorArray = monitorInfo.Update();
    Array monitors;

    int monitorIndex;
    for(const auto& monitor : monitorArray) {
		auto objMonitor = Ref<Monitor>(memnew(Monitor));
		objMonitor->set_friendly_name(StringName(monitor.FriendlyName.c_str()));
		objMonitor->set_device_name(StringName(monitor.DeviceName.c_str()));
		objMonitor->set_is_primary(monitor.IsPrimary);

		Array resolutions;
		for(const auto& res : monitor.Resolutions) {
			auto screenResolution = Ref<ScreenResolution>(memnew(ScreenResolution));
			screenResolution->set_size(Vector2i(res.Dimensions.x, res.Dimensions.y));
			screenResolution->set_refresh_rate(res.RefreshRate);
			resolutions.push_back(Ref(screenResolution));
		}
		objMonitor->set_resolutions(resolutions);

		monitors.push_back(objMonitor);
	}

    return monitors;
}

void MonitorInfo::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_info"), &MonitorInfo::get_info);
}