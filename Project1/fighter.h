#ifndef FIGHTER_H
#define FIGHTER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <set>

struct FighterMovement
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

class Fighter
{
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 worldUp;
	
	float speed;
	float roll;
	float pitch;
	float yaw;
	float rollSpeed;
	float pitchSpeed;
	float yawSpeed;
	float push;
	float pushSpeed;

	static constexpr float DefaultSpeed = 1.5f;
	static constexpr float DefaultRollSpeed = 30.0f;
	static constexpr float DefaultPitchSpeed = 10.0f;
	static constexpr float DefaultYawSpeed = 30.0f;
	static constexpr float DefaultPushSpeed = 0.1f;
	void update();

public:
	Fighter(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f)) :
		position(position), front(front), worldUp(0.0f, 1.0f, 0.0f),
		speed(DefaultSpeed), rollSpeed(DefaultRollSpeed), pitchSpeed(DefaultPitchSpeed), yawSpeed(DefaultYawSpeed),
		pushSpeed(DefaultPushSpeed),
		roll(0.0f), pitch(0.0f), push(0.0f), mode(0)
	{
		update();
	}

	glm::vec3 getPosition() const
	{
		return position;
	}
	glm::vec3 getFront() const{
		return front;
	}
	void processKeyboardMovement(FighterMovement direction);
	//void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
	//void processMouseScroll(float yOffset);
	glm::mat4 model();
	glm::mat4 getRealFront() const;
	int mode;
};

class Fighters {
public:
	Fighter* fighter;
	std::set<Fighter*> fighters;
	static constexpr int MaxFighterNum = 5;
	Fighters(Fighter* fighter) : fighter(fighter)
	{
	}
	void update();
};
#endif