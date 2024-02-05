#include "Renderer.h"
#include "Common.h"

Renderer::Renderer()
{
    //genBuffers();
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) {
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::render() {
    for (auto& batch : m_Batches) {
        //if (has_Buffer_Changed || m_Batches.size() != 1) {
            batch->setBuffers(*m_VertexBuffer, *m_IndexBuffer);
        //}
        Draw(*m_VertexArray, *m_IndexBuffer, *m_PassesShader);
    }
}

void Renderer::addBatch(Batch* batch) {
    m_Batches.push_back(batch);
}

void Renderer::Clear() const{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::genBuffers() {
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
}
