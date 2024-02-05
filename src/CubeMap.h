#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Common.h"
#include "Image.h"
#include "pch.h"

//TODO add bound slot member var

class CubeMap {
private:
	std::unique_ptr<Image> m_Images[6];
	uint32_t m_RendererID;
	std::vector<std::string> m_FilePaths;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	CubeMap() = default;
	CubeMap(const std::string& path);
	~CubeMap();

	void Bind(uint32_t slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};
