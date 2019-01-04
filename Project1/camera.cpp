#include "camera.h"
#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>


void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
	static float DeltaTime = 0, LastFrame = 0;
	float currentFrame = glfwGetTime();
	DeltaTime = currentFrame - LastFrame;
	LastFrame = currentFrame;

	//yaw *= pow(0.9f, DeltaTime);

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
