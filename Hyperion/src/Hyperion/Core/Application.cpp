#include "Application.h"

#include <functional>
#include <chrono>

#include "Log.h"
#include "../Events/ApplicationEvent.h"
#include "../Events/Event.h"

#include "Layer.h"
#include "Window.h"
#include "Hyperion/Renderer/Renderer.h"


namespace Hyperion
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::m_Instance = nullptr;

    Application::Application()
    {
        HYPERION_ASSERT(m_Instance == nullptr, "Application already initialized");
        m_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(event);
            if (event.Handled)
                break;
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PopLayer(Layer* layer)
    {
        m_LayerStack.PopLayer(layer);
        layer->OnDetach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::PopOverlay(Layer* overlay)
    {
        m_LayerStack.PopOverlay(overlay);
        overlay->OnDetach();
    }


    void Application::Run()
    {
        using clock = std::chrono::steady_clock;
        static const auto s_StartTime = clock::now();

        while (m_Running)
        {
            float currentTime = std::chrono::duration<float>(clock::now() - s_StartTime).count();
            Timestep timestep(currentTime - m_LastFrameTime);
            m_LastFrameTime = currentTime;

            if (!m_Minimized)
            {
                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate(timestep);
            }

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
            {
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& /*event*/)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& event)
    {
        if (event.GetWidth() == 0 || event.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }
        m_Minimized = false;
        return false;
    }
}
