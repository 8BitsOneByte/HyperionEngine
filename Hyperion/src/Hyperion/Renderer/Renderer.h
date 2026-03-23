//
// Created by 13191 on 2026/3/15.
//

#ifndef HYPERIONENGINE_RENDERER_H
#define HYPERIONENGINE_RENDERER_H

#include "OrthographicCamera.h"
#include "Shader.h"
#include "Hyperion/Renderer/RenderCommand.h"
#include "Hyperion/Core/Core.h"

namespace Hyperion
{
    class HYPERION_API Renderer
    {
    public:
        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();

        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

        inline static RendererAPI::API GetAPI()
        {
            return RendererAPI::GetAPI();
        }

    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData s_SceneData;
    };
} // Hyperion

#endif //HYPERIONENGINE_RENDERER_H
