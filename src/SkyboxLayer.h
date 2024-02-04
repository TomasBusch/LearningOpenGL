#pragma once
#include <array>
#include <memory>

#include "Layer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Camera.h"
#include "Renderer.h"
#include "CubeMap.h"

class SkyboxLayer : public Layer {
private:
    Renderer m_Renderer;

    std::vector<float> m_SkyboxVertices;
    std::vector<uint32_t> m_SkyboxIndices;

    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<CubeMap> m_Skybox;

    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<VertexBufferLayout> m_VertexBufferLayout;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;

public:
    Camera* m_Camera;

    SkyboxLayer();

    void onUpdate() override;

};