#include "camera.h"
#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>
#include "sync.h"

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity * 0.1;
	yaw -= xOffset;
	pitch -= yOffset;
	if (pitch > 2.0f)
		pitch = 2.0f;
	if (pitch < 0.1f)
		pitch = 0.1f;
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

void Camera::update() {
	while (yaw > 180.0f) yaw -= 360.0f;
	while (yaw < -180.0f) yaw += 360.0f;
	yaw *= pow(0.9f, getDeltaTime());
}