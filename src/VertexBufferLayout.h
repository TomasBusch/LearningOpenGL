#pragma once

#include <vector>
#include <stdexcept>
#include <iostream>

#include <GLEW/glew.h>
#include <glm/glm.hpp>

#include "Common.h"
#include "Types.h"

struct VertexBufferElement {
	uint32_t type;
	uint32_t count;
	unsigned char normalized;
	int size;
	const void* offset;

	static uint32_t GetSizeOfType(uint32_t type);
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> m_Elements;
	uint32_t m_Stride;
public:
	VertexBufferLayout() 
		: m_Stride(0) {

	}

	template<typename T>
	void Push(uint32_t count, const void* offset) {
		/* Does not work on VS2022 (it checks when parsing rather than when instantiating) */
		/*
		static_assert(false);
		*/
		std::runtime_error(false);
	}

	template<>
	void Push<glm::uint8>(uint32_t count, const void* offset) {
		m_Elements.push_back(VertexBufferElement{ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	template<>
	void Push<glm::uint16>(uint32_t count, const void* offset) {
		m_Elements.push_back(VertexBufferElement{ GL_UNSIGNED_SHORT, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_SHORT);
	}

	template<>
	void Push<glm::uint32>(uint32_t count, const void* offset) {
		m_Elements.push_back(VertexBufferElement{ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<glm::int8>(uint32_t count, const void* offset) {
		m_Elements.push_back(VertexBufferElement{ GL_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_BYTE);
	}

	template<>
	void Push<glm::int16>(uint32_t count, const void* offset) {
		m_Elements.push_back(VertexBufferElement{ GL_SHORT, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_SHORT);
	}

	template<>
	void Push<glm::int32>(uint32_t count, const void* offset) {
		m_Elements.push_back(VertexBufferElement{ GL_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_INT);
	}

	template<>
	void Push<glm::float32>(uint32_t count, const void* offset) {
		m_Elements.push_back(VertexBufferElement{ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<glm::vec2>(uint32_t count, const void* offset) {
		count *= 2;
		m_Elements.push_back(VertexBufferElement{ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<glm::vec3>(uint32_t count, const void* offset) {
		count *= 3;
		m_Elements.push_back(VertexBufferElement{ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<glm::vec4>(uint32_t count, const void* offset) {
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

	inline uint32_t GetStride() const { 
		return m_Stride; 
	}
};
