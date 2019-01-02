#include "camera.h"
#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

static float DeltaTime = 0, LastFrame = 0;

void Camera::updateCameraVectors()
{
	fighterFront = glm::normalize(glm::angleAxis(glm::radians(-roll * DeltaTime), worldUp) * fighterFront);
	fighterPosition += movementSpeed * DeltaTime * fighterFront;
	fighterPosition -= movementSpeed * DeltaTime * glm::radians(pitch) * worldUp;
	cameraPosition = fighterPosition + worldUp * 1.0f - fighterFront * 1.5f;
}

void synchronizeMovementSpeed()
{
	float currentFrame = glfwGetTime();
	DeltaTime = currentFrame - LastFrame;
	LastFrame = currentFrame;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(cameraPosition, fighterPosition + worldUp * 0.5f , worldUp);
}


void Camera::processKeyboardMovement(CameraMovement direction)
{
	if (direction.Movement & CameraMovement::FORWARD) {
		pitch += pitchSpeed * DeltaTime;
		if (pitch > 45.0f) pitch = 45.0f;
	}
	if (direction.Movement & CameraMovement::BACKWARD) {
		pitch -= pitchSpeed * DeltaTime;
		if (pitch < -45.0f) pitch = -45.0f;
	}
	if (direction.Movement & CameraMovement::LEFT_ROLL) {
		roll -= rollSpeed * DeltaTime;
		if (roll < -45.0f) roll = -45.0f;
	}
	if (direction.Movement & CameraMovement::RIGHT_ROLL) {
		roll += rollSpeed * DeltaTime;
		if (roll > 45.0f) roll = 45.0f;
	}
	updateCameraVectors();
}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{

		//xOffset *= mouseSensitivity;
		//yOffset *= mouseSensitivity;
		//rotate *= glm::angleAxis(glm::degrees(-xOffset), CameraUp);
		//rotate *= glm::angleAxis(glm::degrees(yOffset), CameraRight);
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
glm::mat4 Camera::model() {
	glm::mat4 model(1.0f);
	model = glm::translate(model, fighterPosition);
	std::cout << fighterPosition.x << " " << fighterPosition.y << " " << fighterPosition.z << " " << std::endl;
	model = glm::rotate(model, glm::radians(pitch), glm::cross(worldUp, fighterFront));
	model = glm::rotate(model, glm::radians(roll), fighterFront);
	model = glm::rotate(model, atan2f(fighterFront.x, fighterFront.z), worldUp);
	model = glm::scale(model, glm::vec3(0.1f));
	return model;
}