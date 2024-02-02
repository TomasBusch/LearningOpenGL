#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "Renderer.h"
#include "Camera.h"
#include "Types.h"

class Application {
private:
	GLFWwindow* m_Window;
	std::string m_GLSL_Version;
	uint32_t m_Width;
	uint32_t m_Height;

	Camera* m_Camera;
	Renderer m_Renderer;
public:
	Application();
	Application(const std::string GLSLVersion, uint32_t width, uint32_t height);
	Application(const std::string GLSL_Version, uint32_t width, uint32_t height, const std::string windowName);
	int Init(const std::string windowName);
	int Run();
private:
	void processInput(GLFWwindow* window, float deltaTime);
	void updateViewportSize(int width, int height);
	void ImGUIMenu(glm::vec3& slider);
};

/*
* How to hide the console?
* Hello, for anyone else with this issue.
* Right click on your project in visual studio.
* Expand the Linker options.
* Click on System.
* Change "SubSystem" to Windows (/SUBSYSTEM:WINDOWS)
* Click on Advanced under the expanded Linker items.
* In the Entry Point box, change it to mainCRTStartup.
* That should force your project to compile as a windows application and not a console application, so the console won't be called.
* 
* for windows:
* 
* #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
* 
* If you don't want to use the pragma, the subsystem can be set in Properties->Linker->System->SubSystem (set it to Windows). 
* The entry point can be set int Properties->Linker->Advanced->Entry Point (set it to mainCRTStartup).
*/