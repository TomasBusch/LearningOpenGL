#pragma once
#include <vector>
#include "Model.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "pch.h"

//TODO keep track of which elements have changed

class Batch {
private:
	std::vector<Model*> m_Models;

	std::vector<Vertex> m_Vertices;
	std::vector<uint32_t> m_Indices;

	bool is_Empty;
	bool has_Changed;
public:
	Batch();

	bool isEmpty() {
		return is_Empty;
	}

	bool hasChanged() {
		return has_Changed;
	}

	void addModel(Model* model);

	void generateBatch();
	void setBuffers(VertexBuffer& vb, IndexBuffer& ib);
};
