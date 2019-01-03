#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cmath>
#include <iostream>
#include "fighter.h"

static float DeltaTime = 0, LastFrame = 0;
void synchronizeMovementSpeed()
{
	float currentFrame = glfwGetTime();
	DeltaTime = currentFrame - LastFrame;
	LastFrame = currentFrame;
}

void Fighter::processKeyboardMovement(FighterMovement direction)
{
	if (direction.Movement & FighterMovement::UPWARD) {
		pitch += pitchSpeed * DeltaTime;
		if (pitch > 45.0f) pitch = 45.0f;
	}
	if (direction.Movement & FighterMovement::DOWNWARD) {
		pitch -= pitchSpeed * DeltaTime;
		if (pitch < -45.0f) pitch = -45.0f;
	}
	if (direction.Movement & FighterMovement::LEFT_ROLL) {
		roll -= rollSpeed * DeltaTime;
		if (roll < -45.0f) roll = -45.0f;
	}
	if (direction.Movement & FighterMovement::RIGHT_ROLL) {
		roll += rollSpeed * DeltaTime;
		if (roll > 45.0f) roll = 45.0f;
	}
	if (direction.Movement & FighterMovement::FORWARD) {
		push += pushSpeed * DeltaTime;
		if (push > 1.0f) push = 1.0f;
	}
	if (direction.Movement & FighterMovement::BACKWARD) {
		push -= pushSpeed * DeltaTime;
		if (push < -1.0f) push = -1.0f;
	}
	if (direction.Movement & FighterMovement::LEFT_SHIFT) {
		yaw += yawSpeed * DeltaTime;
		if (yaw > 10.0f) yaw = 10.0f;
	}
	if (direction.Movement & FighterMovement::RIGHT_SHIFT) {
		yaw -= yawSpeed * DeltaTime;
		if (yaw < -10.0f) yaw = 10.0f;
	}
	update();
}
glm::mat4 Fighter::model() {
	glm::mat4 model(1.0f);
	model = glm::translate(model, position);
	std::cout << position.x << " " << position.y << " " << position.z << " " << std::endl;
	model = glm::rotate(model, glm::radians(-pitch), glm::cross(worldUp, front));
	model = glm::rotate(model, glm::radians(roll), front);
	model = glm::rotate(model, atan2f(front.x, front.z), worldUp);
	model = glm::scale(model, glm::vec3(0.1f));
	return model;
}
void Fighter::update() {
	yaw *= pow(0.5f, DeltaTime);
	front = glm::normalize(
		glm::angleAxis(glm::radians(DeltaTime * (yaw - roll)), worldUp) * front
	);
	speed += push * DeltaTime;
	if (speed < 1.0f) speed = 1.0f;
	position += front * speed * DeltaTime;
	position += worldUp * glm::sin(glm::radians(pitch)) * speed * DeltaTime;
	position += glm::cross(front, worldUp) * glm::sin(glm::radians(yaw)) * speed * DeltaTime;
}
