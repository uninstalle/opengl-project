#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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
	glm::vec3 CameraUp;
	glm::vec3 CameraFront;
	glm::vec3 CameraRight;
	glm::vec3 WorldUp;
	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;
	glm::quat rotate;
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	static constexpr float DefaultSpeed = 1.5f;
	static constexpr float DefaultSensitivity = 0.0001f;
	static constexpr float DefaultZoom = 45.0f;

	void updateCameraVectors();
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f)
			):
		position(position),  up(up), front(front), rotate(glm::quat(1, 0, 0, 0)),
		movementSpeed(DefaultSpeed), mouseSensitivity(DefaultSensitivity), zoom(DefaultZoom)
	{
		updateCameraVectors();
	}
	glm::mat4 GetViewMatrix() const;
	glm::vec3 getPosition() const
	{
		return position;
	}
	float getZoom() const
	{
		return zoom;
	}
	glm::vec3 getFront() {
		return CameraFront;
	}
	void processKeyboardMovement(CameraMovement direction);
	void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void processMouseScroll(float yOffset);
};