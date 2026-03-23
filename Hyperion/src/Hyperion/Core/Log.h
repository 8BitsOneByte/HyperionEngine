#pragma once

#include "../hyperionpch.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Core.h"

namespace Hyperion
{
    class HYPERION_API Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
        static bool s_Initialized;
    };
}

#define HYPERION_CORE_TRACE(...)  ::Hyperion::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HYPERION_CORE_INFO(...)   ::Hyperion::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HYPERION_CORE_WARN(...)   ::Hyperion::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HYPERION_CORE_ERROR(...)  ::Hyperion::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HYPERION_CORE_FATAL(...)  ::Hyperion::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define HYPERION_TRACE(...)       ::Hyperion::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HYPERION_INFO(...)        ::Hyperion::Log::GetClientLogger()->info(__VA_ARGS__)
#define HYPERION_WARN(...)        ::Hyperion::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HYPERION_ERROR(...)       ::Hyperion::Log::GetClientLogger()->error(__VA_ARGS__)
#define HYPERION_FATAL(...)       ::Hyperion::Log::GetClientLogger()->critical(__VA_ARGS__)