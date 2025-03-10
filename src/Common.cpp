#include "Common.h"

#include <iostream>


void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
};

bool GLLogCall(const char* function, const char* file, int line) {
    bool result = true;

    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error]: " << error << " in " << function << " - " << file << " : " << line << std::endl;
        result = false;
    }
    return result;
}