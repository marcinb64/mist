# libmist

## About

A collection of utilities for C++ game dev.

 * Math utilities
 * Implementation of the Observable pattern
 * 2D array class
 * Noise generators
 * Map generation tools

## Dependencies

 * Catch2 for unit testing

## Build

    mkdir -p build
    conan install --build=missing --output-folder=build conanfile.txt -s build_type=Debug
    source build/conanbuild.sh
    cmake -B build --toolchain build/conan_toolchain.cmake 
    cmake --build build
