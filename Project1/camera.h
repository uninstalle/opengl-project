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
	glm::vec3 fighterPosition;
	glm::vec3 fighterFront;
	glm::vec3 worldUp;
	glm::vec3 cameraPosition;
	float roll;
	float rollSpeed;
	float pitch;
	float pitchSpeed;
	float movementSpeed;
	float zoom;

	static constexpr float DefaultSpeed = 1.5f;
	static constexpr float DefaultRollSpeed = 10.0f;
	static constexpr float DefaultPitchSpeed = 10.0f;
	static constexpr float DefaultZoom = 45.0f;

	void updateCameraVectors();
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f)
		) :
		fighterPosition(position), worldUp(up), fighterFront(front),
		movementSpeed(DefaultSpeed), rollSpeed(DefaultRollSpeed), pitchSpeed(DefaultPitchSpeed), zoom(DefaultZoom),
		roll(0.0f), pitch(0.0f)
	{
		updateCameraVectors();
	}
	glm::mat4 GetViewMatrix() const;
	glm::vec3 getPosition() const
	{
		return cameraPosition;
	}
	float getZoom() const
	{
		return zoom;
	}
	glm::vec3 getFront() {
		return fighterPosition - cameraPosition + worldUp;
	}
	void processKeyboardMovement(CameraMovement direction);
	void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void processMouseScroll(float yOffset);
	glm::mat4 model();
};