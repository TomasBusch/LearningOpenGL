#include "SceneLayer.h"

SceneLayer::SceneLayer(uint32_t width, uint32_t height, uint32_t max_vertices, uint32_t max_indices)
	:MAX_VERTICES(max_vertices), MAX_INDICES(max_indices),
	has_Changed(true), has_Buffer_Changed(true),
	m_Camera(new Camera(width, height)),
	m_VertexArray(nullptr), m_VertexBuffer(nullptr), m_VertexBufferLayout(nullptr), m_IndexBuffer(nullptr)
{
	//TODO camera logic

	m_VertexArray = std::make_unique<VertexArray>();
	m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, MAX_VERTICES * sizeof(Vertex));
	m_VertexBufferLayout = std::make_unique<VertexBufferLayout>();
	m_IndexBuffer = std::make_unique<IndexBuffer>(nullptr, MAX_INDICES);

	m_VertexBufferLayout->Push<glm::vec3>(1, (const void*)offsetof(Vertex, Position));
	m_VertexBufferLayout->Push<glm::vec4>(1, (const void*)offsetof(Vertex, Color));
	m_VertexBufferLayout->Push<glm::vec3>(1, (const void*)offsetof(Vertex, Normal));
	m_VertexBufferLayout->Push<glm::vec2>(1, (const void*)offsetof(Vertex, UV));
	m_VertexBufferLayout->Push<glm::float32>(1, (const void*)offsetof(Vertex, MaterialID));

	m_VertexArray->AddBuffer(*m_VertexBuffer, *m_VertexBufferLayout);

	//TODO make path a constructor input
	//m_Shader = std::make_unique<Shader>("res/shaders/basic");
	m_ShaderGeometry = std::make_unique<Shader>("res/shaders/deferred/geometry");
	m_ShaderShading = std::make_unique<Shader>("res/shaders/deferred/shading");

	Texture* textureAlbedo = new Texture("res/textures/test/container2.png");
	Texture* textureSpecular = new Texture("res/textures/test/container2_specular.png");

	textureAlbedo->Bind(5);
	textureSpecular->Bind(6);

	m_ShaderGeometry->Bind();
	m_ShaderGeometry->SetUniform1i("material.albedo", 5);
	m_ShaderGeometry->SetUniform1i("material.roughness", 6);
	//m_ShaderGeometry->SetUniform1f("material.shininess", 128.0f);
	glm::mat4 model_Matrix = glm::mat4(1.0f);
	m_ShaderGeometry->SetUniformMat4f("u_Model", model_Matrix);

	m_ShaderShading->Bind();
	m_ShaderShading->SetUniform3f("dirLight.direction", glm::vec3(-0.5f, -0.5f, -0.5f));
	m_ShaderShading->SetUniform3f("dirLight.ambient", glm::vec3(0.005f, 0.005f, 0.005f));
	m_ShaderShading->SetUniform3f("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	m_ShaderShading->SetUniform3f("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	m_ShaderShading->SetUniform1i("PositionBuffer", 0);
	m_ShaderShading->SetUniform1i("NormalBuffer", 1);
	m_ShaderShading->SetUniform1i("AlbedoRoughBuffer", 2);

	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			m_ShaderShading->SetUniform3f("pointLights[" + std::to_string(i) + "].position", glm::vec3(0.7f, 0.2f, 2.0f));
		}
		if (i == 1) {
			m_ShaderShading->SetUniform3f("pointLights[" + std::to_string(i) + "].position", glm::vec3(-0.7f, 0.2f, 2.0f));
		}
		if (i == 2) {
			m_ShaderShading->SetUniform3f("pointLights[" + std::to_string(i) + "].position", glm::vec3(1.7f, 2.2f, 2.0f));
		}
		if (i == 3) {
			m_ShaderShading->SetUniform3f("pointLights[" + std::to_string(i) + "].position", glm::vec3(-1.7f, 0.2f, -2.0f));
		}
		m_ShaderShading->SetUniform1f("pointLights[" + std::to_string(i) + "].constant", 1.0f);
		m_ShaderShading->SetUniform1f("pointLights[" + std::to_string(i) + "].linear", 0.09f);
		m_ShaderShading->SetUniform1f("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);

		m_ShaderShading->SetUniform3f("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		m_ShaderShading->SetUniform3f("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		m_ShaderShading->SetUniform3f("pointLights[" + std::to_string(i) + "].specular", glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

SceneLayer::~SceneLayer() {
}

void SceneLayer::addModel(Model* model) {
	m_Models.emplace_back(std::make_pair(false, model));
	has_Changed = true;
}

void SceneLayer::onUpdate() {
	m_ShaderGeometry->Bind();
	m_ShaderGeometry->SetUniformMat4f("u_Projection", m_Camera->getProjectionMatrix());
	m_ShaderGeometry->SetUniformMat4f("u_View", m_Camera->getViewMatrix());

	m_ShaderShading->Bind();
	m_ShaderShading->SetUniform3f("u_ViewPos", m_Camera->getPosition());

	m_ShaderShading->SetUniform3f("spotLight.position", m_Camera->getPosition());
	m_ShaderShading->SetUniform3f("spotLight.direction", m_Camera->getDirection());
	m_ShaderShading->SetUniform1f("spotLight.cutOff", glm::cos(glm::radians(15.0f)));
	m_ShaderShading->SetUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(25.0f)));

	m_ShaderShading->SetUniform3f("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	m_ShaderShading->SetUniform3f("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	m_ShaderShading->SetUniform3f("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	m_ShaderShading->SetUniform1f("spotLight.constant", 1.0f);
	m_ShaderShading->SetUniform1f("spotLight.linear", 0.09f);
	m_ShaderShading->SetUniform1f("spotLight.quadratic", 0.032f);

	batchModels();

	for (auto& batch : m_Batches) {
		if (has_Buffer_Changed || m_Batches.size() != 1) {
			batch->setBuffers(*m_VertexBuffer, *m_IndexBuffer);
		}
		m_SRenderer->Draw(*m_VertexArray, *m_IndexBuffer, *m_ShaderGeometry, *m_ShaderShading);
	}
	has_Buffer_Changed = false;

}

void SceneLayer::batchModels() {
	if (has_Changed) {
		m_ModelsQueue = m_Models;

		while (!m_ModelsQueue.empty()) {

			auto batch = std::make_unique<Batch>();

			uint32_t vertex_count = 0;
			uint32_t index_count = 0;

			uint32_t temp_v_count = 0;
			uint32_t temp_i_count = 0;

			for (auto& modelPair : m_ModelsQueue) {
				Model* model = modelPair.second;
				temp_v_count = vertex_count;
				temp_i_count = index_count;

				for (auto& mesh : model->getMeshes()) {
					vertex_count += mesh.getVertices().size();
					index_count += mesh.getIndices().size();
				}
				if (vertex_count <= MAX_VERTICES && index_count <= MAX_INDICES) {
					batch->addModel(model);
					modelPair.first = true;
				}
				else {
					if (temp_v_count == 0) {
						modelPair.first = true;
						std::cout << "Model too big" << std::endl;
					}
					vertex_count = temp_v_count;
					index_count = temp_i_count;
				}
			}
			auto it = m_ModelsQueue.begin();
			while (it != m_ModelsQueue.end()) {
				if (it->first) {
					it = m_ModelsQueue.erase(it);
				}
				else {
					it++;
				}
			}

			//TODO test making this multi threaded
			batch->generateBatch();

			m_Batches.emplace_back(std::move(batch));

			has_Changed = false;
			has_Buffer_Changed = true;
		}
	}
}
