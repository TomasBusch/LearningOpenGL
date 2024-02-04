#pragma once

#include <vector>
#include <memory>

#include "Layer.h"

#include "Renderer.h"
#include "Model.h"
#include "Batch.h"
#include "Camera.h"
#include "Types.h"

class SceneLayer : public Layer {
private:
	Renderer m_Renderer;
	uint32_t MAX_VERTICES;
	uint32_t MAX_INDICES;

	std::vector<std::pair<bool, Model*>> m_Models;
	std::vector<std::unique_ptr<Batch>> m_Batches;
	std::vector<std::pair<bool, Model*>> m_ModelsQueue;
	bool has_Changed;
	bool has_Buffer_Changed;

	std::unique_ptr<VertexArray> m_VertexArray;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<VertexBufferLayout> m_VertexBufferLayout;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;

	std::unique_ptr<Shader> m_Shader;
public:
	//TODO Change to camera array with active camera system
	Camera* m_Camera;

	SceneLayer(uint32_t width, uint32_t height, uint32_t max_vertices, uint32_t max_indices);
	~SceneLayer();

	void addModel(Model* model);

	Camera* getCamera() {
		return m_Camera;
	}


	void onUpdate() override;

private:
	void batchModels();
};
