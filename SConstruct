#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append( CPPPATH=["extension/src/"] )
sources = Glob("extension/src/*.cpp")

if env["platform"] == "windows":
    env.Append( CPPFLAGS=['/D"UNICODE"', '/D"_UNICODE"'] )
    # env['CPDBFLAGS'] = '/Zi /Fd${TARGET}.pdb'
    library = env.SharedLibrary(
        "game/bin/libgdmonitorinfo{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
