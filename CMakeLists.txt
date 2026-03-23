cmake_minimum_required(VERSION 3.16)

project(HyperionEngine LANGUAGES CXX)

# Global build knobs
option(HYPERION_BUILD_SHARED "Build Hyperion as a shared library" ON)
option(HYPERION_ENABLE_LTO "Enable link-time optimization" OFF)
option(HYPERION_ENABLE_UNITY "Enable unity/jumbo builds" OFF)
option(HYPERION_ENABLE_PCH "Enable precompiled headers" ON)
option(HYPERION_ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(HYPERION_ENABLE_SANITIZERS "Enable sanitizers (Debug-only, clang/clang-cl)" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# Consistent binary output layout
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(HyperionCommon)

# Prefer locally checked-out vendor trees when present; fall back to installed packages.
if (EXISTS "${CMAKE_SOURCE_DIR}/vendor/glm/CMakeLists.txt")
    add_subdirectory(vendor/glm)
else()
    find_package(glm CONFIG REQUIRED)
endif()

if (EXISTS "${CMAKE_SOURCE_DIR}/vendor/spdlog/CMakeLists.txt")
    add_subdirectory(vendor/spdlog)
else()
    find_package(spdlog CONFIG REQUIRED)
endif()

if (EXISTS "${CMAKE_SOURCE_DIR}/vendor/glfw/CMakeLists.txt")
    add_subdirectory(vendor/glfw)
else()
    find_package(glfw3 CONFIG REQUIRED)
endif()

add_subdirectory(Hyperion)
add_subdirectory(Sandbox)
