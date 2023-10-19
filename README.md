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

Example build commands, using conan profiles `linux-debug` and `linux-release`.

    conan install conanfile.py --build=missing --profile linux-debug
    conan install conanfile.py --build=missing --profile linux-release
    cmake --preset conan-debug
    cmake --build build/Debug

    conan create . --profile=linux-release
    conan create . --profile=linux-debug

