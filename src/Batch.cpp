#include "Batch.h"
#include <iostream>
#include <chrono>

Batch::Batch()
	:is_Empty(true), has_Changed(true), MAX_VERTICES(pow(2, 15)), MAX_INDICES(pow(2, 15))
{

}

Batch::Batch(unsigned int max_vertices, unsigned int max_indices)
	:is_Empty(true), has_Changed(true), MAX_VERTICES(max_vertices), MAX_INDICES(max_indices)
{

}

void Batch::addModel(Model* model) {
	m_Models.push_back(std::make_pair(false ,model));
	is_Empty = false;
	has_Changed = true;
}

void Batch::begin() {
	m_ModelsQueue = m_Models;
	is_Empty = false;
}

void Batch::generateDrawQueue() {
	unsigned int vertex_count = 0;
	unsigned int index_count = 0;

	unsigned int temp_v_count = 0;
	unsigned int temp_i_count = 0;

	for (auto& modelPair : m_ModelsQueue) {
		Model* model = modelPair.second;
		temp_v_count = vertex_count;
		temp_i_count = index_count;

		for (auto& mesh : model->getMeshes()) {
			vertex_count += mesh.getVertices().size();
			index_count += mesh.getIndices().size();
		}
		if (vertex_count <= MAX_VERTICES && index_count <= MAX_INDICES) {
			m_ModelsDrawQueue.push_back(model);
			modelPair.first = true;
		}
		else {
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
	if (m_ModelsQueue.empty()) {
		is_Empty = true;
	}
}

void Batch::generateBatch(VertexBuffer& vb, IndexBuffer& ib) {
	unsigned int vertex_offset = 0;
	unsigned int index_offset = 0;
	std::vector<unsigned int> indices;
	indices.reserve(MAX_INDICES);

	for (auto& model : m_ModelsDrawQueue) {
		for (auto& mesh : model->getMeshes()) {
			vb.SetData(vertex_offset * sizeof(Vertex), sizeof(Vertex) * mesh.getVertices().size(), mesh.getVertices().data());

			for (unsigned int index : mesh.getIndices()) {
				indices.push_back(index + vertex_offset);
			//ib.SetData(index_offset * sizeof(unsigned int), sizeof(unsigned int) * mesh.getIndices().size(), mesh.getIndices().data());
			}

			vertex_offset += mesh.getVertices().size();
			//index_offset += mesh.getIndices().size();
		}
	}

	ib.SetData(0, sizeof(unsigned int) * indices.size(), indices.data());

	m_ModelsDrawQueue.clear();

	has_Changed = false;
}
