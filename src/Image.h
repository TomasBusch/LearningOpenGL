#pragma once
#include <string>

#include "pch.h"

class Image {
private:
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Image() = default;
	Image(const std::string& path, bool flip = true);
	~Image();

	unsigned char* getDataBuffer() const {
		return m_LocalBuffer;
	}
	int getWidth() const {
		return m_Width;
	}
	int getHeight() const {
		return m_Height;
	}
};
