#ifndef AMMUNITION_H
#define AMMUNITION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <set>
#include "fighter.h"


class Ammunition {
private:
	Fighter *fighter;
	glm::vec3 position;
	glm::mat4 fmodel;
	float speed;

	static constexpr float DefaultSpeed = 5.0f;
	
	
public:
	Ammunition(Fighter* fighter) : fighter(fighter), speed(DefaultSpeed)
	{
		position = fighter->getPosition();
		fmodel = fighter->getRealFront();
	}
	glm::vec3 getPosition() const {
		return position;
	}
	Fighter* getFighter() const {
		return fighter;
	}
	void update();
	glm::mat4 model();
};


class Ammunitions {
public:
	std::set<Ammunition*> ammunitions;
	void update();
	void processMouse(Fighter* fighter);
};
#endif
