#pragma once

#include <vector>
#include <memory>

#include "Renderer.h"
#include "Model.h"
#include "Batch.h"
#include "Camera.h"
#include "Types.h"

class Scene {
private:
	Renderer m_Renderer;
	uint32_t MAX_VERTICES;
	uint32_t MAX_INDICES;

	std::vector<std::pair<bool, Model*>> m_Models;
	std::vector<std::unique_ptr<Batch>> m_Batches;
	std::vector<std::pair<bool, Model*>> m_ModelsQueue;
	bool has_Changed;

	std::unique_ptr<VertexArray> m_VertexArray;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<VertexBufferLayout> m_VertexBufferLayout;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;

	std::unique_ptr<Shader> m_Shader;

	//TODO Change to camera array with active camera system
	Camera m_Camera;
public:
	Scene(uint32_t width, uint32_t height, uint32_t max_vertices, uint32_t max_indices);
	~Scene();

	void addModel(Model* model);

	void draw();

	Camera* getCamera() {
		return &m_Camera;
	}

private:
	void batchModels();
};
