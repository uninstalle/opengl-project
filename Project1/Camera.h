#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

struct CameraMovement
{
	enum
	{
		FORWARD = 1,
		BACKWARD = 2,
		LEFT_SHIFT = 4,
		RIGHT_SHIFT = 8,
		LEFT_ROLL = 16,
		RIGHT_ROLL = 32,
		UPWARD = 64,
		DOWNWARD = 128
	};
	unsigned Movement = 0;
};

struct CameraTransformMatrix
{
	glm::mat4 view;
	glm::mat4 projection;
};

void synchronizeMovementSpeed();


class Camera
{
private:

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 worldUp;
	glm::vec3 cameraUp;
	glm::vec3 right;
	float yaw;
	float pitch;
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	static constexpr float DefaultYaw = -90.0f;
	static constexpr float DefaultPitch = 0.0f;
	static constexpr float DefaultSpeed = 1.5f;
	static constexpr float DefaultSensitivity = 0.1f;
	static constexpr float DefaultZoom = 45.0f;

	void updateCameraVectors();
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = DefaultYaw, float pitch = DefaultPitch) 
	: position(position), front(glm::vec3(0.0f, 0.0f, -1.0f)), worldUp(up),yaw(yaw),pitch(pitch), movementSpeed(DefaultSpeed), mouseSensitivity(DefaultSensitivity), zoom(DefaultZoom)
	{
		updateCameraVectors();
	}
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
	: position(glm::vec3(posX, posY, posZ)), front(glm::vec3(0.0f, 0.0f, -1.0f)), worldUp(glm::vec3(upX, upY, upZ)), yaw(yaw), pitch(pitch), movementSpeed(DefaultSpeed), mouseSensitivity(DefaultSensitivity), zoom(DefaultZoom)
	{
		updateCameraVectors();
	}
	glm::mat4 GetViewMatrix() const;
	glm::vec3 getPosition() const
	{
		return position;
	}
	glm::vec3 getFront() const
	{
		return front;
	}
	glm::vec3 getUp() const
	{
		return cameraUp;
	}
	float getZoom() const
	{
		return zoom;
	}
	void processKeyboardMovement(CameraMovement direction);
	void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void processMouseScroll(float yOffset);
};