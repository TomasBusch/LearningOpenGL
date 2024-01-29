#include "Shader.h"
#include <GLEW/glew.h>
#include "Common.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& path) 
	: m_FilePath(path), m_RendererID(0)
{
    ShaderProgramSource shaderSource = ReadShaders(path);
    m_RendererID = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);

    Bind();
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const {
   GLCall(glUseProgram(m_RendererID));
}

void Shader::Ubind() const {
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int v0) {
    GLCall(glUniform1i(GetUniformLocation(name), v0));
}

void Shader::SetUniform1f(const std::string& name, float v0) {
    GLCall(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform1iv(const std::string& name, const unsigned int count, int values[]) {
    GLCall(glUniform1iv(GetUniformLocation(name), count, values));
}

unsigned int Shader::GetUniformLocation(const std::string& name) {
    auto cached_location = m_UniformLocationCache.find(name);
    if (cached_location != m_UniformLocationCache.end()) {
        return cached_location->second;
    }
    else {
        int location = glGetUniformLocation(m_RendererID, name.c_str());
        m_UniformLocationCache[name] = location;
        if (location == -1) {
            std::cout << "Warning: Binding to a non existent uniform (" << name << ")" << std::endl;
        }
        return location;
    }
}
 

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)_malloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "failed to compile shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

ShaderProgramSource Shader::ReadShaders(const std::string& filepath) {
    std::ifstream fragmentStream(filepath + "/fragment.glsl");
    std::ifstream vertexStream(filepath + "/vertex.glsl");

    std::string line;
    std::stringstream strFragmentStream;
    std::stringstream strVertexStream;

    while (getline(vertexStream, line)) {
        strVertexStream << line << '\n';
    }

    while (getline(fragmentStream, line)) {
        strFragmentStream << line << '\n';
    }

    return { strVertexStream.str(), strFragmentStream.str() };
}

