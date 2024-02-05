#pragma once

#include <mutex>

#include "pch.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Batch.h"

class SRenderer {
private:
    static SRenderer* pinstance_;
    static std::mutex mutex_;

    uint32_t MAX_VERTICES;
    uint32_t MAX_INDICES;

    uint32_t m_Width, m_Height;

    Shader* m_GeometryS;
    Shader* m_ShadingS;
    uint32_t gBuffer, gPosition, gNormal, gAlbedoRough, rboDepth;

    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<VertexBufferLayout> m_VertexBufferLayout;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::vector<float> m_QuadVertices;
    std::vector<uint32_t> m_QuadIndices;
protected:
    SRenderer(uint32_t width, uint32_t height, uint32_t max_vertices, uint32_t max_indices);
    ~SRenderer() {};
public:
    SRenderer(SRenderer& other) = delete;
    void operator=(const SRenderer&) = delete;
    static SRenderer* GetInstance(uint32_t width, uint32_t height, uint32_t max_vertices, uint32_t max_indices);

    void Draw(const VertexArray& va, const IndexBuffer& ib);
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& geometry_s);
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& geometry_s, const Shader& shading_s);
    void Clear() const;
private:
    void GeometryPass(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
    void LightingPass(const Shader& shader);
    void SetupBuffers();
};


