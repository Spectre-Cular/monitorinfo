/* monitorinfo.h */

#ifndef MONITORINFO_H
#define MONITORINFO_H

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/vector2i.hpp>
#include "monitor_info.h"

using namespace godot;

class ScreenResolution : public RefCounted {
    GDCLASS(ScreenResolution, RefCounted);

public:
    ScreenResolution();
    ~ScreenResolution();

    Vector2i get_size() { return size; }
    uint32_t get_refresh_rate() { return refresh_rate; }
    void set_size(const Vector2i size) { this->size = size; }
    void set_refresh_rate(const uint32_t refresh_rate) { this->refresh_rate = refresh_rate; }

protected:
    static void _bind_methods();

private:
    Vector2i size;
    uint32_t refresh_rate;
};

class Monitor : public RefCounted {
    GDCLASS(Monitor, RefCounted);

public:
    Monitor();
    ~Monitor();

    StringName get_friendly_name() { return friendly_name; }
    StringName get_device_name() { return device_name; }
    bool get_is_primary() { return is_primary; }
    Array get_resolutions() { return resolutions; }

    void set_friendly_name(const StringName& friendly_name) { this->friendly_name = friendly_name; }
    void set_device_name(const StringName& device_name) { this->device_name = device_name; }
    void set_is_primary(const bool is_primary) { this->is_primary = is_primary; }
    void set_resolutions(const Array& resolutions) { this->resolutions = resolutions; }

protected:
    static void _bind_methods();

private:
    StringName friendly_name;
    StringName device_name;
    bool is_primary;
    Array resolutions;
};

class MonitorInfo : public RefCounted {
    GDCLASS(MonitorInfo, RefCounted);

protected:
    static void _bind_methods();

public:
    MonitorInfo();
    ~MonitorInfo();

    Array get_info();

private:
    CMonitorInfo monitorInfo;
};

#endif // MONITORINFO_H