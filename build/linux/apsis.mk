# Source location
SOURCE_PATH=../../src

# Projects we depend on
DEPENDENCIES=json clew sdl_mixer soil ogg vorbis glew freetype

# Libraries
LIBS=m c stdc++ GL dl SDL2 GLU

C_COMPILER=clang
CPP_COMPILER=clang

# Weird thing: doesn't seem to properly link ogg from .a
LINK=objs/${BUILD}/ogg/bitwise objs/${BUILD}/ogg/framing

# Asset path
ASSETS=../../assets

# C++ flags
CPP_FLAGS=-std=c++11

# List of files to not optimize
DO_NOT_OPTIMIZE=sync/atomic_counter.cpp

# List of files to not compile
EXCLUDE=backend/sdl2.cpp

DEFINE=SDL1

# Output
BINARY=apsis
