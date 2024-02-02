#include "IndexBuffer.h"

#include "Common.h"

IndexBuffer::IndexBuffer(const void* data, uint32_t count)
    : m_Count(count) {
    ASSERT(sizeof(uint32_t) == sizeof(GLuint));

    /* generate and bind vertex index buffer */
    GLCall(glGenBuffers(1, &m_RendererID));
    //GLCall(glCreateBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    /* Copy data to buffer */
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STREAM_DRAW));
}

IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::SetData(uint32_t offset, uint32_t size, const void* data) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);

    m_Count = size / sizeof(uint32_t);
}
