#pragma once
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"
#include "Types.h"

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera {
private:
	glm::vec3 m_Position;
	glm::vec3 m_Direction;
	//glm::vec3 m_Target;

	//World Space
	glm::vec3 m_WorldUp;

	//Camera Space
	glm::vec3 m_CameraRight;
	glm::vec3 m_CameraUp;

	//Rotation
	glm::float32 m_Pitch;
	glm::float32 m_Yaw;
	glm::float32 m_Roll;

	//glm::mat4 m_ViewMatrix;

	//Mouse
	glm::float32 m_MouseSensitivity;
	bool m_FirstMouse;
	uint32_t m_ViewportWidth;
	uint32_t m_ViewportHeight;
	glm::float32 m_MouseLastX;
	glm::float32 m_MouseLastY;
	glm::float32 m_MouseZoom;

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_MVPMatrix;

	bool m_Locked;

	//Movement
	uint32_t m_MovSpeed;


public:
	Camera(uint32_t width, uint32_t height);

	glm::mat4 getMVPMatrix() {
		return m_MVPMatrix;
	}

	glm::mat4 getProjectionMatrix() {
		return m_ProjectionMatrix;
	}

	glm::mat4 getViewMatrix() {
		return m_ViewMatrix;
	}

	glm::vec3 getPosition() {
		return m_Position;
	}
	
	glm::vec3 getDirection() {
		return m_Direction;
	}

	void processKeyboard(Camera_Movement direction, float deltaTime);
	void processMouseMovement(float xoffset, float yoffset, bool constrainPitch);
	void ProcessMouseScroll(float yoffset);
	void updateCameraVectors();

	void setViewportSize(uint32_t width, uint32_t height);

	void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void unlock(GLFWwindow* window);
	void lock(GLFWwindow* window);
	void toggleLock(GLFWwindow* window);

private:
	void updateViewMatrix();
	void updateProjectionMatrix();
	void updateMVPMatrix();
};
