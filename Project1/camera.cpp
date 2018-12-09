#include "camera.h"
#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>

static float DeltaTime = 0, LastFrame = 0;

void Camera::updateCameraVectors()
{
	up = rotate * up;
	front = rotate * front;
	up = glm::normalize(up);
	front = glm::normalize(front);
	right = glm::cross(front, up);
	CameraUp = up;
	CameraFront = front;
	CameraRight = right;
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	rotate = glm::quat(1, 0, 0, 0);
}

void synchronizeMovementSpeed()
{
	float currentFrame = glfwGetTime();
	DeltaTime = currentFrame - LastFrame;
	LastFrame = currentFrame;
}

glm::mat4 Camera::GetViewMatrix() const
{
		return glm::lookAt(position, position + CameraFront, CameraUp);
}


void Camera::processKeyboardMovement(CameraMovement direction)
{

	float velocity = movementSpeed * DeltaTime;
	float rollV = 0.01*velocity;
	if (direction.Movement & CameraMovement::FORWARD)
		position += front * velocity;
	if (direction.Movement & CameraMovement::BACKWARD)
		position -= front * velocity;
	if (direction.Movement & CameraMovement::LEFT_SHIFT)
		position -= right * velocity;
	if (direction.Movement & CameraMovement::RIGHT_SHIFT)
		position += right * velocity;
	if (direction.Movement & CameraMovement::UPWARD)
		position += up * velocity;
	if (direction.Movement & CameraMovement::DOWNWARD)
		position -= up * velocity;
	if (direction.Movement & CameraMovement::LEFT_ROLL) {
		rotate *= glm::angleAxis(glm::degrees(-rollV), CameraFront);
	}
	if (direction.Movement & CameraMovement::RIGHT_ROLL) {
		rotate *= glm::angleAxis(glm::degrees(rollV), CameraFront);
	}
	updateCameraVectors();
}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{

		xOffset *= mouseSensitivity;
		yOffset *= mouseSensitivity;
		rotate *= glm::angleAxis(glm::degrees(-xOffset), CameraUp);
		rotate *= glm::angleAxis(glm::degrees(yOffset), CameraRight);
		updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset)
{
		if (zoom >= 1.0f && zoom <= 45.0f)
			zoom -= yOffset;
		if (zoom <= 1.0f)
			zoom = 1.0f;
		if (zoom >= 45.0f)
			zoom = 45.0f;
}