#pragma once

#include "Types.h"

class IndexBuffer {
private:
	uint32_t m_RendererID;
	uint32_t m_Count;
public:
	IndexBuffer(const void* data, uint32_t count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetData(uint32_t offset, uint32_t size, const void* data);

	inline uint32_t GetCount() const { return m_Count; }
};
