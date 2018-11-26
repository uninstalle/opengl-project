#ifndef _SKY_BOX_H
#define _SKY_BOX_H
#include <glm/glm.hpp>

void initializeSkybox();
void drawSkybox(glm::mat4 view, glm::mat4 projection);

#endif