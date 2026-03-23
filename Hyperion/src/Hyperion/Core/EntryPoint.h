#pragma once

#include "Log.h"
#include "Application.h"

extern Hyperion::Application* CreateApplication();

int main(int /*argc*/, char** /*args*/)
{
    Hyperion::Log::Init();
    HYPERION_CORE_INFO("Hyperion Engine starting up");

    auto app = CreateApplication();
    app->Run();
    delete app;
    return 0;
}