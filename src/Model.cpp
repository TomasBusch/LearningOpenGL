#include "Model.h"
#include <iostream>

Material::Material(
	const std::string& albedo_path, 
	const std::string& roughness_path, 
	const std::string& metalness_path, 
	const std::string& height_path, 
	const std::string& normal_path) 
	: m_Albedo  (std::make_unique<Image>(albedo_path   )),
	m_Roughness (std::make_unique<Image>(roughness_path)),
	m_Metalness (std::make_unique<Image>(metalness_path)),
	m_HeightMap (std::make_unique<Image>(height_path   )),
	m_NormalMap (std::make_unique<Image>(normal_path   ))
{

}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Material* material)
	: m_Vertices(vertices), m_Indices(indices), m_Material(material), m_ModelMatrix(glm::mat4(1.0f))
{

}

Mesh::~Mesh() {
	if (m_Material != nullptr) {
		delete m_Material;
	}
}

void Mesh::transformMesh(glm::mat4 transform) {
	m_ModelMatrix = m_ModelMatrix * transform;

	for (auto& vertex : m_Vertices) {
		vertex.Position = m_ModelMatrix * glm::vec4(vertex.Position, 1.0f);
		//Multiply by the inverse transpose matrix
		vertex.Normal = glm::normalize(glm::transpose(glm::inverse(m_ModelMatrix)) * glm::vec4(vertex.Normal, 0.0f));
	}
}

void Mesh::resetMesh() {
	m_ModelMatrix = glm::inverse(m_ModelMatrix);
	for (auto& vertex : m_Vertices) {
		vertex.Position = m_ModelMatrix * glm::vec4(vertex.Position, 1.0f);
		//Multiply by the inverse transpose matrix
		vertex.Normal = glm::normalize(glm::transpose(glm::inverse(m_ModelMatrix)) * glm::vec4(vertex.Normal, 0.0f));
	}
	m_ModelMatrix = glm::mat4(1.0f);
}

Model::Model(const std::string& path) {
	LoadModel(path);
}

void Model::LoadModel(std::string path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	// process all the node's meshes (if any)
	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	Material* material = nullptr;

	float mat_index = 0.0f;

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		aiString str;
		std::string albedo_path;
		std::string roughness_path;
		std::string metalness_path;
		std::string height_path;
		std::string normal_path;

		if (mat->GetTextureCount(aiTextureType_BASE_COLOR) > 0) {
			mat->GetTexture(aiTextureType_BASE_COLOR, 0, &str);
		}
		else {
			str.Set("res/missing_albedo.png");
		}
		albedo_path = str.C_Str();
		if (mat->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0) {
			mat->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &str);
		}
		else {
			str.Set("res/missing_roughness.png");
		}
		roughness_path = str.C_Str();
		if (mat->GetTextureCount(aiTextureType_METALNESS) > 0) {
			mat->GetTexture(aiTextureType_METALNESS, 0, &str);
		}
		else {
			str.Set("res/missing_metalness.png");
		}
		metalness_path = str.C_Str();
		if (mat->GetTextureCount(aiTextureType_HEIGHT) > 0) {
			mat->GetTexture(aiTextureType_HEIGHT, 0, &str);
		}
		else {
			str.Set("res/missing_height.png");
		}
		height_path = str.C_Str();
		if (mat->GetTextureCount(aiTextureType_NORMALS) > 0) {
			mat->GetTexture(aiTextureType_NORMALS, 0, &str);
		}
		else {
			str.Set("res/missing_normal.png");
		}
		normal_path = str.C_Str();

		material = new Material(albedo_path, roughness_path, metalness_path, height_path, normal_path);
		mat_index = mesh->mMaterialIndex;
	}
	
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;

		vertex.Position = position;

		glm::vec3 normals;
		normals.x = mesh->mNormals[i].x;
		normals.y = mesh->mNormals[i].y;
		normals.z = mesh->mNormals[i].z;

		vertex.Normal = normals;

		if (mesh->HasTextureCoords(0)) {
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.UV = vec;
		}else {
			vertex.UV = glm::vec2(0.0f, 0.0f);
		}

		if (mesh->HasVertexColors(0)) {
			vertex.Color.r = mesh->mColors[i]->r;
			vertex.Color.g = mesh->mColors[i]->g;
			vertex.Color.b = mesh->mColors[i]->b;
			vertex.Color.a = mesh->mColors[i]->a;
		}
		else {
			vertex.Color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}

		vertex.MaterialID = mat_index;

		vertices.push_back(vertex);
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	
	return Mesh(vertices, indices, material);
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




