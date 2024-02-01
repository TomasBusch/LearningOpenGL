#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

Camera::Camera(int width, int height)
	:m_Position(glm::vec3(0.0f, 0.0f, 0.0f)), m_WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), m_Yaw(-90.0f), m_Pitch(0.0f), m_Roll(0.0f),
	m_Direction(glm::vec3(0.0f, 0.0f, -1.0f)), m_FirstMouse(true), m_MouseLastX(0), m_MouseLastY(0), m_MouseSensitivity(0.5f) , m_MouseZoom(45.0f),
	m_ViewportWidth(width), m_ViewportHeight(height),
	m_ProjectionMatrix(glm::mat4(1.0f)), m_ViewMatrix(glm::mat4(1.0f)), m_MVPMatrix(glm::mat4(1.0f)),
	m_Locked(false)
{
	m_CameraRight = glm::normalize(glm::cross(m_WorldUp, m_Direction));
	m_CameraUp = glm::cross(m_Direction, m_CameraRight);

	m_ProjectionMatrix = glm::perspective(glm::radians(m_MouseZoom), (float)m_ViewportWidth / (float)m_ViewportHeight, 0.1f, 10000.0f);
	m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Direction, m_CameraUp);
	m_MVPMatrix = m_ProjectionMatrix * m_ViewMatrix;

	updateViewMatrix();
	updateProjectionMatrix();
	updateMVPMatrix();
	updateCameraVectors();
}

void Camera::updateViewMatrix() {
	m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Direction, m_CameraUp);
}

void Camera::updateProjectionMatrix() {
	m_ProjectionMatrix = glm::perspective(glm::radians(m_MouseZoom), (float)m_ViewportWidth / (float)m_ViewportHeight, 0.1f, 10000.0f);
}

void Camera::updateMVPMatrix() {
	m_MVPMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Camera::processKeyboard(Camera_Movement movement, float deltaTime) {
	if (!m_Locked) {

		//float velocity = MovementSpeed * deltaTime;
		float velocity = 2 * deltaTime;
		if (movement == FORWARD)
			m_Position += m_Direction * velocity;
		if (movement == BACKWARD)
			m_Position -= m_Direction * velocity;
		if (movement == LEFT)
			m_Position -= m_CameraRight * velocity;
		if (movement == RIGHT)
			m_Position += m_CameraRight * velocity;
		if (movement == UP)
			m_Position += m_CameraUp * velocity;
		if (movement == DOWN)
			m_Position -= m_CameraUp * velocity;

		updateViewMatrix();
		updateMVPMatrix();
	}
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {

	xoffset *= m_MouseSensitivity;
	yoffset *= m_MouseSensitivity;

	m_Yaw += xoffset;
	m_Pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
	updateViewMatrix();
	updateMVPMatrix();
}

void Camera::ProcessMouseScroll(float yoffset) {
	m_MouseZoom -= (float)yoffset * 10;
	if (m_MouseZoom < 1.0f)
		m_MouseZoom = 1.0f;
	if (m_MouseZoom > 45.0f)
		m_MouseZoom = 45.0f;

	updateProjectionMatrix();
	updateMVPMatrix();
}

void Camera::updateCameraVectors() {
	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	direction.y = sin(glm::radians(m_Pitch));
	direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Direction = glm::normalize(direction);

	m_CameraRight = glm::normalize(glm::cross(m_Direction, m_WorldUp));
	m_CameraUp = glm::normalize(glm::cross(m_CameraRight, m_Direction));
}

void Camera::setViewportSize(unsigned int width, unsigned int height) {
	m_ViewportWidth = width;
	m_ViewportHeight = height;
}

void Camera::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	if (!m_Locked) {

		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		float xoffset = xpos - m_ViewportWidth / 2;
		float yoffset = (m_ViewportHeight / 2) - ypos; // reversed since y-coordinates go from bottom to top

		//Avoids jump in first move
		if (m_FirstMouse) {
			m_FirstMouse = false;
		}
		else {
			processMouseMovement(xoffset, yoffset);
		}

		//TODO GLFW_CURSOR_DISABLED should recenter the cursor automatically
		glfwSetCursorPos(window, (float)m_ViewportWidth / 2, (float)m_ViewportHeight / 2);
	}
}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if(!m_Locked){
		ProcessMouseScroll(static_cast<float>(yoffset));
	}
}

void Camera::unlock(GLFWwindow* window) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	m_Locked = false;
}

void Camera::lock(GLFWwindow* window) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	m_Locked = true;
	m_FirstMouse = true;
}

void Camera::toggleLock(GLFWwindow* window) {
	if (!m_Locked) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	m_FirstMouse = true;
	m_Locked = !m_Locked;
}
