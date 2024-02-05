#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#include "pch.h"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	std::string m_FilePath;
	uint32_t m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader() = default;
	Shader(const std::string& path);
	~Shader();

	void Bind() const;
	void Ubind() const;

	//Set Uniforms
	void SetUniform1i(const std::string& name, int v0);
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform3f(const std::string& name, glm::vec3 values);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform4f(const std::string& name, glm::vec4 values);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniform1iv(const std::string& name, const uint32_t count, int values[]);
private:
	uint32_t CompileShader(uint32_t type, const std::string& source);
	uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource ReadShaders(const std::string& filepath);

	uint32_t GetUniformLocation(const std::string& name);
};