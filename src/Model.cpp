#include "Model.h"

Material::Material(
	const std::string& albedo_path, 
	const std::string& roughness_path, 
	const std::string& metallic_path, 
	const std::string& height_path, 
	const std::string& normal_path) 
	: m_Albedo  (std::make_unique<Image>(albedo_path   )),
	m_Roughness (std::make_unique<Image>(roughness_path)),
	m_Metallic  (std::make_unique<Image>(metallic_path )),
	m_HeightMap (std::make_unique<Image>(height_path   )),
	m_NormalMap (std::make_unique<Image>(normal_path   ))
{

}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, Material* material)
	: m_Vertices(vertices), m_Indices(indices), m_Material(material), m_ModelMatrix(glm::mat4(1.0f))
{

}

void Mesh::transformMesh(glm::mat4 transform) {
	m_ModelMatrix = m_ModelMatrix * transform;

	for (auto& vertex : m_Vertices) {
		vertex.Position = m_ModelMatrix * glm::vec4(vertex.Position, 1.0f);
		//TODO Multiply by the inverse transpose matrix
		vertex.Normal = glm::normalize(glm::transpose(glm::inverse(m_ModelMatrix)) * glm::vec4(vertex.Normal, 0.0f));
	}
}

void Mesh::resetMesh() {
	m_ModelMatrix = glm::inverse(m_ModelMatrix);
	for (auto& vertex : m_Vertices) {
		vertex.Position = m_ModelMatrix * glm::vec4(vertex.Position, 1.0f);
		//TODO Multiply by the inverse transpose matrix
		vertex.Normal = glm::normalize(glm::transpose(glm::inverse(m_ModelMatrix)) * glm::vec4(vertex.Normal, 0.0f));
	}
	m_ModelMatrix = glm::mat4(1.0f);
}

Model::Model(const std::string& path) {
	//Todo Load obj files
}

void Model::transformModel(glm::mat4 transform) {
	for (auto& mesh : m_Meshes) {
		mesh.transformMesh(transform);
	}
}

void Model::resetModel() {
	for (auto& mesh : m_Meshes) {
		mesh.resetMesh();
	}
}




