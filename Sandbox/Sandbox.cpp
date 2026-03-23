#include "../Hyperion/src/Hyperion.h"


class ExampleLayer : public Hyperion::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
    {
        m_VertexArray.reset(Hyperion::VertexArray::Create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.1f, 0.8f, 0.2f, 0.8f, 1.0f,
            0.5f, -0.5f, 0.1f, 0.2f, 0.3f, 0.8f, 1.0f,
            0.0f, 0.5f, 0.1f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        std::shared_ptr<Hyperion::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Hyperion::VertexBuffer::Create(vertices, sizeof(vertices)));

        Hyperion::BufferLayout layout = {
            {Hyperion::ShaderDataType::Float3, "a_Position"},
            {Hyperion::ShaderDataType::Float4, "a_Color"}
        };

        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = {0, 1, 2};
        std::shared_ptr<Hyperion::IndexBuffer> indexBuffer;
        indexBuffer.reset(Hyperion::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        m_SquareVertexArray.reset(Hyperion::VertexArray::Create());

        float squareVertices[3 * 4] = {
            -0.75f, -0.75f, 0.0f,
            0.75f, -0.75f, 0.0f,
            0.75f, 0.75f, 0.0f,
            -0.75f, 0.75f, 0.0f
        };

        std::shared_ptr<Hyperion::VertexBuffer> squareVertexBuffer;
        squareVertexBuffer.reset(Hyperion::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        Hyperion::BufferLayout squareVertexBufferLayout = {
            {Hyperion::ShaderDataType::Float3, "a_Position"},
        };
        squareVertexBuffer->SetLayout(squareVertexBufferLayout);
        m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<Hyperion::IndexBuffer> squareIndexBuffer;
        squareIndexBuffer.reset(Hyperion::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position;
            out vec4 v_Color;


            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main()
            {
                color = v_Color;
            }
        )";

        m_Shader.reset(new Hyperion::Shader(vertexSrc, fragmentSrc));

        std::string blueShaderVertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position;

            void main()
            {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";

        std::string blueShaderFragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;

            void main()
            {
                color = vec4(0.2, 0.3, 0.8, 1.0);
            }
        )";

        m_BlueShader.reset(new Hyperion::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
    }

    void OnUpdate(Hyperion::Timestep /*ts*/) override
    {
        //HYPERION_INFO("ExampleLayer::Update");

        // if (Hyperion::Input::IsKeyPressed(HYPERION_KEY_TAB))
        // {
        //     HYPERION_TRACE("Tab key is pressed!");
        // }

        Hyperion::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Hyperion::RenderCommand::Clear();

        m_Camera.SetPosition({0.5f, 0.5f, 0.0f});
        m_Camera.SetRotation(45.0f);

        Hyperion::Renderer::BeginScene(m_Camera);


        Hyperion::Renderer::Submit(m_BlueShader, m_SquareVertexArray);
        Hyperion::Renderer::Submit(m_Shader, m_VertexArray);


        Hyperion::Renderer::EndScene();

    }

    virtual void OnImGuiRender() override
    {

    }

    void OnEvent(Hyperion::Event& event) override
    {
        if (event.GetEventType() == Hyperion::EventType::KeyPressed)
        {
            auto& keyEvent = static_cast<Hyperion::KeyPressedEvent&>(event);
            HYPERION_TRACE("Key Pressed: {} ", keyEvent.GetKeyCode());
        }
    };

private:
    std::shared_ptr<Hyperion::VertexArray> m_VertexArray;
    std::shared_ptr<Hyperion::Shader> m_Shader;

    std::shared_ptr<Hyperion::VertexArray> m_SquareVertexArray;
    std::shared_ptr<Hyperion::Shader> m_BlueShader;

    Hyperion::OrthographicCamera m_Camera;
};


class Sandbox : public Hyperion::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer);
    }

    ~Sandbox() override
    {
    }
};

Hyperion::Application* CreateApplication()
{
    return new Sandbox();
}