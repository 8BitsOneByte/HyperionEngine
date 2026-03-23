//
// Created by 13191 on 2026/3/16.
//

#ifndef HYPERIONENGINE_OPENGLVERTEXARRAY_H
#define HYPERIONENGINE_OPENGLVERTEXARRAY_H
#include "Hyperion/Renderer/VertexArray.h"

namespace Hyperion
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

        virtual std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() { return m_VertexBuffers; };
        virtual std::shared_ptr<IndexBuffer>& GetIndexBuffer() { return m_IndexBuffers; };

    private:
        uint32_t m_RendererID;
        uint32_t m_VertexBufferIndex = 0;
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffers;
    };
} // Hyperion

#endif //HYPERIONENGINE_OPENGLVERTEXARRAY_H