//
// Created by 13191 on 2026/1/4.
//
#pragma once

#include "../Core/Layer.h"
#include "../Events/Event.h"

#include "Hyperion/Events/MouseEvent.h"
#include "Hyperion/Events/KeyEvent.h"
#include "Hyperion/Events/ApplicationEvent.h"

namespace Hyperion
{
    class HYPERION_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();
    };
} // Hyperion