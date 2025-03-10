#include "Texture.h"

#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path) 
	:m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), m_Image(path)
{
	GLCall(glGenTextures(1, &m_RendererID));
	//GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	//Linear Gamma
	//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Image.getWidth(), m_Image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Image.getDataBuffer()));
	//2.2 Gamma (srgb)
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, m_Image.getWidth(), m_Image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Image.getDataBuffer()));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture() {
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(uint32_t slot) const {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
