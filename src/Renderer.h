#pragma once

#include "pch.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "pch.h"
#include "Batch.h"

class Renderer {
private:
    uint32_t m_Width, m_Height;

    std::vector<Batch*> m_Batches;

    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<VertexBufferLayout> m_VertexBufferLayout;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;

    std::unique_ptr<Shader> m_PassesShader;
    uint32_t gBuffer, gPosition, gNormal, gAlbedoRough;

public:
    Renderer();
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
    void render();
    void addBatch(Batch* batch);
    void Clear() const;
private:
    void genBuffers();
};
