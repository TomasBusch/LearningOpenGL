#pragma once

#include "glm/glm.hpp"
#include <array>
#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec3 Normal;
	glm::vec2 UV;
	glm::float32 TextureID;
};

class Material {

};

class Mesh {
private:
	std::vector<Vertex> m_Verteces;
	std::vector<unsigned int> m_Indices;
	Material m_Material;
};

class Model {
private:
	std::vector<Mesh> m_Meshes;
};