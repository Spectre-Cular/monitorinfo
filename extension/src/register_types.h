/* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#ifndef MONITORINFO_REGISTER_TYPES_H
#define MONITORINFO_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void initialize_monitorinfo_module(ModuleInitializationLevel p_level);
void uninitialize_monitorinfo_module(ModuleInitializationLevel p_level);

#endif // MONITORINFO_REGISTER_TYPES_H
