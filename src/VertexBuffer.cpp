#include "Common.h"

#include "VertexBuffer.h"
#include "Model.h"

VertexBuffer::VertexBuffer(const void* data, uint32_t size) {
    /* Generate and bind vertex buffer */
    GLCall(glGenBuffers(1, &m_RendererID));
    //GLCall(glCreateBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    /* Copy data to buffer */
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW));
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::SetData(uint32_t offset, uint32_t size, const void* data) {
    /* Set dynamic Vertex Buffer Data*/
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}
