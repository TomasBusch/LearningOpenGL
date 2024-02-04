#pragma once
#include <string>

#include "Types.h"

class Image {
private:
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Image() = default;
	Image(const std::string& path, int flip = 1);
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
