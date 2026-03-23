//
// Created by 13191 on 2026/3/15.
//

#include "Hyperion/hyperionpch.h"
#include "Renderer.h"

namespace Hyperion
{
    Renderer::SceneData Renderer::s_SceneData;

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        s_SceneData.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
    {
        shader->Bind();
        shader->UploadUniformMat4("u_ViewProjection", s_SceneData.ViewProjectionMatrix);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
} // Hyperion
