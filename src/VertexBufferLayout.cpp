#include "VertexBufferLayout.h"

uint32_t VertexBufferElement::GetSizeOfType(uint32_t type) {
	switch (type) {
	case GL_BYTE:           return 1;
	case GL_UNSIGNED_BYTE:  return 1;
	case GL_SHORT:          return 2;
	case GL_UNSIGNED_SHORT: return 2;
	case GL_INT:            return 4;
	case GL_UNSIGNED_INT:   return 4;
	case GL_FLOAT:			return 4;
	}
	ASSERT(false);
	return 0;
}
