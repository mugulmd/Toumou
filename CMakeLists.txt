message("CMake version: ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}")
cmake_minimum_required(VERSION 3.25)
project(Toumou LANGUAGES CXX VERSION 0.1.0)


# Build options
option(BUILD_TOUMOU_ENGINE "Build the Toumou engine as a shared library" ON)
option(BUILD_EXAMPLE_APP "Build the Toumou example application" ON)
option(BUILD_PYTHON_BINDINGS "Build the Toumou python bindings" ON)


# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# For generating shared libs
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


# Dependencies
set(TOUMOU_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# pybind11
find_package(pybind11 REQUIRED)

# spdlog
find_package(spdlog REQUIRED)

# OpenEXR
find_package(OpenEXR REQUIRED)


# Builds
add_subdirectory(src)
