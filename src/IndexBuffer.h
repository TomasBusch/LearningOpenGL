#pragma once

class IndexBuffer {
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const void* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetData(unsigned int offset, unsigned int size, const void* data);

	inline unsigned int GetCount() const { return m_Count; }
};
