#include "VertexBuffer.h"

#include "Common.h"
#include "Model.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    /* Generate and bind vertex buffer */
    GLCall(glGenBuffers(1, &m_RendererID));
    //GLCall(glCreateBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    /* Copy data to buffer */
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
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

void VertexBuffer::SetData(unsigned int offset, unsigned int size, const void* data) {
    /* Set dynamic Vertex Buffer Data*/
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}
