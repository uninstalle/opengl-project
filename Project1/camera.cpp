#include "camera.h"
#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>
static float DeltaTime = 0, LastFrame = 0;

void Camera::updateCameraVectors()
{
	rotation = glm::normalize(rotationX * rotationZ * rotationY);
	cameraUp = rotation * initialUp;
	cameraFront = rotation * initialFront;
	cameraRight = rotation * glm::cross(initialFront, initialUp);
}

void synchronizeMovementSpeed()
{
	float currentFrame = glfwGetTime();
	DeltaTime = currentFrame - LastFrame;
	LastFrame = currentFrame;
}

glm::mat4 Camera::GetViewMatrix() const
{
		return glm::lookAt(position, position + cameraFront, cameraUp);
}


void Camera::processKeyboardMovement(CameraMovement direction)
{

	float velocity = movementSpeed * DeltaTime;
	if (direction.Movement & CameraMovement::FORWARD)
		position += cameraFront * velocity;
	if (direction.Movement & CameraMovement::BACKWARD)
		position -= cameraFront * velocity;
	if (direction.Movement & CameraMovement::LEFT_SHIFT)
		position -= cameraRight * velocity;
	if (direction.Movement & CameraMovement::RIGHT_SHIFT)
		position += cameraRight * velocity;
	if (direction.Movement & CameraMovement::UPWARD)
		position += cameraUp * velocity;
	if (direction.Movement & CameraMovement::DOWNWARD)
		position -= cameraUp * velocity;
	if (direction.Movement & CameraMovement::LEFT_ROLL) {
		rotationZ *= glm::angleAxis(glm::degrees(-rotationSpeed), initialFront);
	}
	if (direction.Movement & CameraMovement::RIGHT_ROLL) {
		rotationZ *= glm::angleAxis(glm::degrees(rotationSpeed), initialFront);
	}
	updateCameraVectors();
}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{

	rotationX = glm::angleAxis(glm::degrees(rotationSpeed * yOffset), glm::cross(initialFront, initialUp)) * rotationX;
	rotationY = rotationY * glm::angleAxis(glm::degrees(rotationSpeed * -xOffset), initialUp);
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