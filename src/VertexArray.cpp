#include "VertexArray.h"
#include "Common.h"

VertexArray::VertexArray() {
	/* Generate and bind vertex array objects */
	GLCall(glGenVertexArrays(1, &m_RendererID));
	//GLCall(glCreateVertexArrays(1, &m_RendererID));
	GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	intptr_t offset = 0;
	for (uint32_t i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		/* Specify vertex attribute types */
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}
