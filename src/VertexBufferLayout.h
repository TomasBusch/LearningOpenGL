#pragma once

#include <vector>
#include <GLEW/glew.h>
#include "Common.h"
#include "glm/glm.hpp"
#include <stdexcept>
#include <iostream>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
	int size;
	const void* offset;

	static unsigned int GetSizeOfType(unsigned int type);
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout() 
		: m_Stride(0) {

	}

	template<typename T>
	void Push(unsigned int count, const void* offset) {
		/* Does not work on VS2022 (it checks when parsing rather than when instantiating) */
		/*
		static_assert(false);
		*/
		std::runtime_error(false);
	}

	template<>
	void Push<glm::uint8>(unsigned int count, const void* offset) {
		m_Elements.push_back(VertexBufferElement{ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	template<>
	void Push<glm::uint16>(unsigned int count, const void* offset) {
		m_Elements.push_back(VertexBufferElement{ GL_UNSIGNED_SHORT, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_SHORT);
	}

	template<>
	void Push<glm::uint32>(unsigned int count, const void* offset) {
		m_Elements.push_back(VertexBufferElement{ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<glm::int8>(unsigned int count, const void* offset) {
		m_Elements.push_back(VertexBufferElement{ GL_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_BYTE);
	}

	template<>
	void Push<glm::int16>(unsigned int count, const void* offset) {
		m_Elements.push_back(VertexBufferElement{ GL_SHORT, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_SHORT);
	}

	template<>
	void Push<glm::int32>(unsigned int count, const void* offset) {
		m_Elements.push_back(VertexBufferElement{ GL_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_INT);
	}

	template<>
	void Push<glm::float32>(unsigned int count, const void* offset) {
		m_Elements.push_back(VertexBufferElement{ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<glm::vec2>(unsigned int count, const void* offset) {
		count *= 2;
		m_Elements.push_back(VertexBufferElement{ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<glm::vec3>(unsigned int count, const void* offset) {
		count *= 3;
		m_Elements.push_back(VertexBufferElement{ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<glm::vec4>(unsigned int count, const void* offset) {
		count *= 4;
		m_Elements.push_back(VertexBufferElement{ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}


	/* Applied fix to return a const& instead of a const copy */
	/*                                           |            */
	/*                                           v            */
	inline const std::vector<VertexBufferElement>& GetElements() const {
		return m_Elements;
	}

	inline unsigned int GetStride() const { 
		return m_Stride; 
	}
};
