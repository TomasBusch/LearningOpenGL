#include "CubeMap.h"

#include "stb_image/stb_image.h"

CubeMap::CubeMap(const std::string& path)
	:m_RendererID(0), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	GLCall(glGenTextures(1, &m_RendererID));
	//GLCall(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));

	for (int i = 0; i < 6; i++) {
		m_FilePaths.push_back(path + "/" + std::to_string(i) + ".jpg");
	}

	for (uint32_t j = 0; j < m_FilePaths.size(); j++) {

		m_Images[j] = std::make_unique<Image>(m_FilePaths[j], false);
		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_RGBA8,
			m_Images[j]->getWidth(), m_Images[j]->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
			m_Images[j]->getDataBuffer()));
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//Linear Gamma
	//GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP, 0, GL_RGBA8, m_Image.getWidth(), m_Image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Image.getDataBuffer()));
	//2.2 Gamma (srgb)
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

CubeMap::~CubeMap() {
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void CubeMap::Bind(uint32_t slot) const {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
}

void CubeMap::Unbind() const {
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}
