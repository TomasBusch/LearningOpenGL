#pragma once

#include "pch.h"

class VertexBuffer {
private:
	uint32_t m_RendererID;
public:
	VertexBuffer(const void* data, uint32_t size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetData(uint32_t offset, uint32_t size, const void* data);
};
