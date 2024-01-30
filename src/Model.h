#pragma once

#include "glm/glm.hpp"
#include <array>
#include <vector>
#include <memory>
#include "Texture.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec3 Normal;
	glm::vec2 UV;
	glm::float32 TextureID;
};

class Material {
private:
	std::unique_ptr<Image> m_Albedo;
	std::unique_ptr<Image> m_Roughness;
	std::unique_ptr<Image> m_Metallic;
	std::unique_ptr<Image> m_HeightMap;
	std::unique_ptr<Image> m_NormalMap;
public:
	Material(
		const std::string& albedo_path, 
		const std::string& roughness_path, 
		const std::string& metallic_path, 
		const std::string& height_path, 
		const std::string& normal_path);


};

class Mesh {
private:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	Material* m_Material;

	glm::mat4 m_ModelMatrix;
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, Material* material);

	const std::vector<Vertex>& getVertices() const {
		return m_Vertices;
	}
	const std::vector<unsigned int>& getIndices() const {
		return m_Indices;
	}

	void transformMesh(glm::mat4 transform);

	void resetMesh();
};

class Model {
private:
	std::vector<Mesh> m_Meshes;
public:
	Model(const std::string& path);

	const std::vector<Mesh>& getMeshes() const {
		return m_Meshes;
	}

	void transformModel(glm::mat4 transform);

	void resetModel();
};