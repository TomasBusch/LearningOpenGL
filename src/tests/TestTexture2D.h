#pragma once

#include "Test.h"
#include <VertexArray.h>
#include <IndexBuffer.h>
#include <Shader.h>
#include <Texture.h>
#include <Renderer.h>

namespace test {
	class TestTexture2D : public Test {
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		Renderer m_Renderer;

		VertexArray* m_VertexArray;
		VertexBuffer* m_VertexBuffer;
		VertexBufferLayout* m_VertexBufferLayout;
		IndexBuffer* m_IndexBuffer;

		Shader* m_Shader;
		Texture* m_Texture;

		glm::vec3 m_TranslationA;
		glm::vec3 m_TranslationB;

		glm::mat4 m_ModelA;
		glm::mat4 m_ModelB;

		glm::mat4 m_VPMatrix;
		glm::mat4 m_MVP;
	};
}