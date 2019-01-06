#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "ammunition.h"
#include "sync.h"

void Ammunition::update() {
	glm::vec4 front = fmodel * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f) * speed * getDeltaTime();
	position += glm::vec3(front.x, front.y, front.z);
}

glm::mat4 Ammunition::model() {
	glm::mat4 model(1.0f);
	model = glm::translate(model, position);
	model *= fmodel;
	model = glm::scale(model, glm::vec3(0.01f));
	return model;
}

void Ammunitions::update() {
	for (auto it = ammunitions.begin(); it != ammunitions.end();) {
		if (glm::distance((*it)->getPosition(), (*it)->getFighter()->getPosition()) > 30.0f) {
			delete *it;
			it = ammunitions.erase(it);
			continue;
		}
		(*it)->update();
		it++;
	}
}
void Ammunitions::processMouse(Fighter* fighter) {
	ammunitions.insert(new Ammunition(fighter));
}