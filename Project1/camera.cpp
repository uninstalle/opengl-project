#include "camera.h"
#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>

static float DeltaTime = 0, LastFrame = 0;

void synchronizeMovementSpeed()
{
	float currentFrame = glfwGetTime();
	DeltaTime = currentFrame - LastFrame;
	LastFrame = currentFrame;
}

void Camera::updateCameraVectors()
{
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		this->front = glm::normalize(front);
		right = glm::normalize(glm::cross(this->front, worldUp));
		up = glm::normalize(glm::cross(right, this->front));
}

glm::mat4 Camera::GetViewMatrix() const
{
		return glm::lookAt(position, position + front, up);
}


void Camera::processKeyboardMovement(CameraMovement direction)
{

	float velocity = movementSpeed * DeltaTime;
	if (direction.Movement & CameraMovement::FORWARD)
		position += front * velocity;
	if (direction.Movement & CameraMovement::BACKWARD)
		position -= front * velocity;
	if (direction.Movement & CameraMovement::LEFT_SHIFT)
		position -= right * velocity;
	if (direction.Movement & CameraMovement::RIGHT_SHIFT)
		position += right * velocity;

}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{

		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (constrainPitch)
		{
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}

		updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
		if (zoom >= 1.0f && zoom <= 45.0f)
			zoom -= yoffset;
		if (zoom <= 1.0f)
			zoom = 1.0f;
		if (zoom >= 45.0f)
			zoom = 45.0f;
}
