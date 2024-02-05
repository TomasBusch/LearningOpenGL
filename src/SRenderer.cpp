#include "SRenderer.h"

SRenderer::SRenderer(uint32_t width, uint32_t height, uint32_t max_vertices, uint32_t max_indices)
    :MAX_VERTICES(max_vertices), MAX_INDICES(max_indices), m_Width(width), m_Height(height),
    m_VertexArray(nullptr), m_VertexBuffer(nullptr), m_VertexBufferLayout(nullptr), m_IndexBuffer(nullptr)
{
    m_QuadVertices = {
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    m_QuadIndices = {
        3, 2, 0,
        0, 1, 3,
    };

    m_VertexArray = std::make_unique<VertexArray>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(m_QuadVertices.data(), 4 * (3 + 2) * sizeof(float));
    m_VertexBufferLayout = std::make_unique<VertexBufferLayout>();
    m_IndexBuffer = std::make_unique<IndexBuffer>(m_QuadIndices.data(), 6);

    m_VertexBufferLayout->Push<glm::vec3>(1, (void*)0);
    m_VertexBufferLayout->Push<glm::vec2>(1, (void*)(3 * sizeof(float)));

    m_VertexArray->AddBuffer(*m_VertexBuffer, *m_VertexBufferLayout);


    m_GeometryS = new Shader("res/shaders/deferred/geometry");
    m_ShadingS = new Shader("res/shaders/deferred/shading");

    SetupBuffers();
}

void SRenderer::Draw(const VertexArray& va, const IndexBuffer& ib) {
    Draw(va, ib, *m_GeometryS);
}

void SRenderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& geometry_s) {
    Draw(va, ib, geometry_s, *m_ShadingS);
}

void SRenderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& geometry_s, const Shader& shading_s) {
    GeometryPass(va, ib, geometry_s);
    LightingPass(shading_s);
}

void SRenderer::GeometryPass(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) {
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    Clear();
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SRenderer::LightingPass(const Shader& shader) {
    Clear();
    shader.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoRough);
    m_VertexArray->Bind();
    m_IndexBuffer->Bind();
    GLCall(glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
    //glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    //glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SRenderer::Clear() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void SRenderer::SetupBuffers() {

    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    GLCall(glGenTextures(1, &gPosition));
    GLCall(glBindTexture(GL_TEXTURE_2D, gPosition));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0));

    GLCall(glGenTextures(1, &gNormal));
    GLCall(glBindTexture(GL_TEXTURE_2D, gNormal));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0));

    GLCall(glGenTextures(1, &gAlbedoRough));
    GLCall(glBindTexture(GL_TEXTURE_2D, gAlbedoRough));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoRough, 0));

    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);
    

    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_ShadingS->Bind();
    m_ShadingS->SetUniform1i("PositionBuffer", 0);
    m_ShadingS->SetUniform1i("NormalBuffer", 1);
    m_ShadingS->SetUniform1i("AlbedoRoughBuffer", 2);
}

SRenderer* SRenderer::pinstance_{ nullptr };
std::mutex SRenderer::mutex_;

SRenderer* SRenderer::GetInstance(uint32_t width, uint32_t height, uint32_t max_vertices, uint32_t max_indices) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new SRenderer(width, height, max_vertices, max_indices);
    }
    return pinstance_;
}