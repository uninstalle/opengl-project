#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "fighter.h"
#include "ammunition.h"
#include <set>
#include "sync.h"

static float DeltaTime = 0, LastFrame = 0;
void synchronizeTime()
{
	float currentFrame = glfwGetTime();
	DeltaTime = currentFrame - LastFrame;
	LastFrame = currentFrame;
}
float getDeltaTime(){
	return DeltaTime;
}
void check(std::set<Fighter*> &fighters, std::set<Ammunition*> &ammunitions) {
	std::set<Fighter*> delf;
	std::set<Ammunition*> dela;
	for (auto p : fighters)
		for (auto q : fighters) 
			if(p != q) {
			if (glm::distance(p->getPosition(), q->getPosition()) < 1.0f) {
				delf.insert(p);
				delf.insert(q);
			}
		}
	for (auto p : fighters)
		for (auto q : ammunitions)
			{
				if (glm::distance(p->getPosition(), q->getPosition()) < 1.0f) {
					delf.insert(p);
					dela.insert(q);
				}
			}
	for (auto p : ammunitions)
		for (auto q : ammunitions)
			if (p != q) {
				if (glm::distance(p->getPosition(), q->getPosition()) < 0.1f) {
					dela.insert(p);
					dela.insert(q);
				}
			}
	for (auto p : delf) fighters.erase(p);
	for (auto p : dela) ammunitions.erase(p);
}