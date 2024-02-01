#pragma once
#include <vector>
#include "Model.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

//TODO keep track of which elements have changed

class Batch {
private:
	std::vector<std::pair<bool, Model*>> m_Models;
	std::vector<std::pair<bool ,Model*>> m_ModelsQueue;
	std::vector<Model*> m_ModelsDrawQueue;
	bool is_Empty;
	bool has_Changed;

	const unsigned int MAX_VERTICES;
	const unsigned int MAX_INDICES;
public:
	Batch();
	Batch(unsigned int MAX_VERTICES, unsigned int MAX_INDICES);

	void addModel(Model* model);

	bool isEmpty() {
		return is_Empty;
	}

	bool hasChanged() {
		return has_Changed;
	}

	void begin();
	void generateDrawQueue();
	void generateBatch(VertexBuffer& vb, IndexBuffer& ib);
};
