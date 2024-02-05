#include "Image.h"
#include <stb_image/stb_image.h>

Image::Image(const std::string& path, bool flip) {
	stbi_set_flip_vertically_on_load(flip);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
}

Image::~Image() {
	if (m_LocalBuffer) {
		stbi_image_free(m_LocalBuffer);
	}
}
