#pragma once

#include <string>

#include "Common.h"
#include "Image.h"
#include "pch.h"

//TODO add bound slot member var

class Texture {
private:
	Image m_Image;
	uint32_t m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture() = default;
	Texture(const std::string& path);
	~Texture();

	void Bind(uint32_t slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};
