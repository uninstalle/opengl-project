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
		if (yaw > 20.0f) yaw = 20.0f;
	}
	if (direction.Movement & FighterMovement::RIGHT_SHIFT) {
		yaw -= yawSpeed * DeltaTime;
		if (yaw < -20.0f) yaw = -20.0f;
	}
	update();
}
glm::mat4 Fighter::model() {
	yaw *= pow(0.5f, DeltaTime);
	glm::mat4 model(1.0f);
	model = glm::translate(model, position);
	//std::cout << position.x << " " << position.y << " " << position.z << " " << std::endl;
	model = glm::rotate(model, glm::radians(-pitch), glm::cross(worldUp, front));
	model = glm::rotate(model, glm::radians(roll), front);
	model = glm::rotate(model, atan2f(front.x, front.z), worldUp);
	model = glm::scale(model, glm::vec3(0.1f));
	return model;
}
void Fighter::update() {
	front = glm::normalize(
		glm::angleAxis(glm::radians(DeltaTime * (yaw - roll)), worldUp) * front
	);
	speed += push * DeltaTime;
	if (speed < 1.0f) speed = 1.0f;
	position += front * speed * DeltaTime;
	position += worldUp * glm::sin(glm::radians(pitch)) * speed * DeltaTime;
	position += glm::cross(front, worldUp) * glm::sin(glm::radians(yaw)) * speed * DeltaTime;
}

void Fighters::update() {
	if (fighters.size() < MaxFighterNum) {
		float rd = float(rand()) / RAND_MAX;
		if (rd < pow(MaxFighterNum - fighters.size(), 1)) {
			glm::vec3 front = glm::vec3(sin(rd * glm::pi<float>() * 2), 0.0f, cos(rd * glm::pi<float>() * 2));
			fighters.insert(new Fighter(fighter->getPosition() - front * 20.0f, front));
		}
	}
	for (auto it = fighters.begin(); it != fighters.end();) {
		if (glm::length((*it)->getPosition() - fighter->getPosition()) > 30.0f){
			delete *it;
			it = fighters.erase(it);
			continue;
		}
		FighterMovement fighterMovement;
		if (fighter->getPosition().y > (*it)->getPosition().y) fighterMovement.Movement |= FighterMovement::UPWARD;
		else fighterMovement.Movement |= FighterMovement::DOWNWARD;
		if (glm::cross((*it)->getFront(), fighter->getPosition() - (*it)->getPosition()).y < 0) fighterMovement.Movement |= FighterMovement::RIGHT_SHIFT;
		else fighterMovement.Movement |= FighterMovement::LEFT_SHIFT;
		(*it)->processKeyboardMovement(fighterMovement);
		it++;
	}
}