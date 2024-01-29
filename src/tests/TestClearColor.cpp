#include "TestClearColor.h"
#include "Common.h"
#include "imgui/imgui.h"

namespace test {

	TestClearColor::TestClearColor()
		: m_ClearColor{ 0.0f, 0.0f, 0.0f, 1.0f } {

	}

	TestClearColor::~TestClearColor() {
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestClearColor::OnUpdate(float deltaTime) {

	}

	void TestClearColor::OnRender() {
		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		m_Renderer.Clear();
	}

	void TestClearColor::OnImGuiRender() {
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}