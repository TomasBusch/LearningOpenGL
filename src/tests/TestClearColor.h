#pragma once

#include "Test.h"
#include <Renderer.h>

namespace test {
	class TestClearColor : public Test {
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		Renderer m_Renderer;
		float m_ClearColor[4];
	};
}