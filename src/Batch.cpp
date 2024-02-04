#include "Batch.h"
#include <iostream>

Batch::Batch()
	:is_Empty(true), has_Changed(true)
{

}

void Batch::addModel(Model* model) {
	m_Models.push_back(model);
}

void Batch::generateBatch() {
	uint32_t index_offset = 0;

	m_Vertices.clear();
	m_Indices.clear();

	for (auto& model : m_Models) {
		for (auto& mesh : model->getMeshes()) {

			for (auto& vertex : mesh.getVertices()) {
				//Vertex* vert = new Vertex();
				m_Vertices.push_back(vertex);
			}

			for (uint32_t index : mesh.getIndices()) {
				m_Indices.push_back(index + index_offset);
			}

			index_offset += mesh.getVertices().size();
		}
	}

	has_Changed = false;
}

void Batch::setBuffers(VertexBuffer& vb, IndexBuffer& ib) {
	vb.SetData(0, sizeof(Vertex) * m_Vertices.size(), m_Vertices.data());
	ib.SetData(0, sizeof(uint32_t) * m_Indices.size(), m_Indices.data());
}
