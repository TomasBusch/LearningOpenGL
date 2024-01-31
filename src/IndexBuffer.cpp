#include "IndexBuffer.h"

#include "Common.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
    : m_Count(count) {
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    /* generate and bind vertex index buffer */
    GLCall(glGenBuffers(1, &m_RendererID));
    //GLCall(glCreateBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    /* Copy data to buffer */
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW));
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

void IndexBuffer::SetData(unsigned int offset, unsigned int size, const void* data) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}
