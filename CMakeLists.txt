cmake_minimum_required(VERSION 3.13)

# Project definition
project(
    Mist
    VERSION 0.1
    LANGUAGES CXX)

# Build customization
set(CMAKE_BUILD_TYPE Debug)
option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" On)
option(ENABLE_CLANG_TIDY "Run clang-tidy during build" Off)
option(ENABLE_SANITIZERS "Enable sanitizers" On)

# Project-specific settings
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

# Pseudo-library for adding compiler warning flags to targets
add_library(project_warnings INTERFACE)
include(CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# Misc options
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
include(StaticAnalysis.cmake)

# Path to conan-installed libraries
#list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/conan)
#list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/conan)

#
# Project structure
#
add_subdirectory(mist)
add_subdirectory(app)
