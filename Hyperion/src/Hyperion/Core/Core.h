#pragma once

// ---------------------------------------------------------------------------
// DLL import/export / visibility
// ---------------------------------------------------------------------------
#ifdef HYPERION_PLATFORM_WINDOWS
    #ifdef HYPERION_STATIC
        #define HYPERION_API
    #elif defined(HYPERION_BUILD_DLL)
        #define HYPERION_API __declspec(dllexport)
    #else
        #define HYPERION_API __declspec(dllimport)
    #endif
#elif defined(HYPERION_PLATFORM_LINUX) || defined(HYPERION_PLATFORM_MACOS)
    #ifdef HYPERION_BUILD_DLL
        #define HYPERION_API __attribute__((visibility("default")))
    #else
        #define HYPERION_API
    #endif
#else
    #error "Unknown platform — define HYPERION_PLATFORM_WINDOWS, HYPERION_PLATFORM_LINUX, or HYPERION_PLATFORM_MACOS"
#endif

// ---------------------------------------------------------------------------
// Cross-platform debug break
// ---------------------------------------------------------------------------
#if defined(_MSC_VER)
    #define HYPERION_DEBUGBREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
    #include <signal.h>
    #define HYPERION_DEBUGBREAK() raise(SIGTRAP)
#else
    #define HYPERION_DEBUGBREAK() ((void)0)
#endif

// ---------------------------------------------------------------------------
// Assertions (enabled when HYPERION_ENABLE_ASSERTS is defined by CMake)
// ---------------------------------------------------------------------------
#ifdef HYPERION_ENABLE_ASSERTS
    #include <cstdio>
    #define HYPERION_CORE_ASSERT(x, ...) \
        do { if (!(x)) { std::fprintf(stderr, "[HYPERION ASSERT] " __VA_ARGS__); std::fprintf(stderr, "\n"); HYPERION_DEBUGBREAK(); } } while (0)
    #define HYPERION_ASSERT(x, ...) \
        do { if (!(x)) { std::fprintf(stderr, "[ASSERT] " __VA_ARGS__); std::fprintf(stderr, "\n"); HYPERION_DEBUGBREAK(); } } while (0)
#else
    #define HYPERION_CORE_ASSERT(x, ...) ((void)(x))
    #define HYPERION_ASSERT(x, ...)      ((void)(x))
#endif

#define BIT(x) (1u << (x))