#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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
	glm::vec3 initialUp, cameraUp;
	glm::vec3 initialFront, cameraFront;
	glm::vec3 cameraRight;
	glm::quat rotation;
	glm::quat rotationX;
	glm::quat rotationY;
	glm::quat rotationZ;
	float yaw;
	float pitch;
	float movementSpeed;
	float rotationSpeed;
	float rotationSensitivity;
	float zoom;

	static constexpr float DefaultMovementSpeed = 1.5f;
	static constexpr float DefaultRotationSpeed = 0.0001f;
	static constexpr float DefaultZoom = 45.0f;

	void updateCameraVectors();
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f)
	) :
		position(position), initialFront(front), initialUp(up), rotation(glm::quat(1, 0, 0, 0)),
		movementSpeed(DefaultMovementSpeed), rotationSpeed(DefaultRotationSpeed), zoom(DefaultZoom)
	{
		rotationX = rotationY = rotationZ = rotation;
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
		return cameraFront;
	}
	void processKeyboardMovement(CameraMovement direction);
	void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void processMouseScroll(float yOffset);
};