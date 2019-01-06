#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "fighter.h"
struct CameraTransformMatrix
{
	glm::mat4 view;
	glm::mat4 projection;
};

class Camera
{
private:
	float zoom;
	float yaw;
	float pitch;
	float mouseSensitivity;
	Fighter* fighter;
	glm::vec3 worldUp;
	static constexpr float DefaultZoom = 45.0f;
	static constexpr float DefaultMouseSensitivity = 0.1f;
public:
	Camera() : zoom(DefaultZoom), worldUp(glm::vec3(0.0f, 1.0f, 0.0f)), yaw(0.0f), pitch(0.0f), mouseSensitivity(DefaultMouseSensitivity)
	{
	}
	glm::mat4 GetViewMatrix() const {
		return glm::lookAt(getPosition(), fighter->getPosition() + worldUp * 0.5f, worldUp);
	}
	glm::vec3 getPosition() const {
		return fighter->getPosition() - glm::normalize(glm::angleAxis(glm::radians(yaw), worldUp) * fighter->getFront()) * 1.5f + worldUp * pitch;
	}
	float getZoom() const
	{
		return zoom;
	}
	glm::vec3 getFront() const{
		return fighter->getPosition() - worldUp * 0.5f - getPosition();
	}
	void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void processMouseScroll(float yOffset);
	void setFighter(Fighter* fighter) {
		this->fighter = fighter;
	}
	void update();
};