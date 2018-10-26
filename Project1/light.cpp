#include "light.h"
#include <glad/glad.h>

void DirLight::apply(Shader& shader)
{
	shader.setVec3f(position, "dirLight.position");
	shader.setVec3f(ambient, "dirLight.ambient");
	shader.setVec3f(diffuse, "dirLight.diffuse");
	shader.setVec3f(specular, "dirLight.specular");
}

int PointLight::PointLightCount = 0;

void PointLight::apply(Shader& shader)
{
	glUniform1i(glGetUniformLocation(shader.getID(), "PointLightNum"), PointLightCount);
	char lightIndex = '0' + PointLightCount - 1;
	std::string plightName("pointLight[0].");
	plightName.at(plightName.find('0')) = lightIndex;
	shader.setVec3f(position, std::string(plightName + "position").c_str());
	shader.setVec3f(ambient, std::string(plightName + "ambient").c_str());
	shader.setVec3f(diffuse, std::string(plightName + "diffuse").c_str());
	shader.setVec3f(specular, std::string(plightName + "specular").c_str());
	glUniform1f(glGetUniformLocation(shader.getID(), std::string(plightName + "constant").c_str()), constant);
	glUniform1f(glGetUniformLocation(shader.getID(), std::string(plightName + "linear").c_str()), linear);
	glUniform1f(glGetUniformLocation(shader.getID(), std::string(plightName + "quadratic").c_str()), quadratic);
}

void SpotLight::apply(Shader& shader)
{
	shader.setVec3f(position, "spotLight.position");
	shader.setVec3f(ambient, "spotLight.ambient");
	shader.setVec3f(diffuse, "spotLight.diffuse");
	shader.setVec3f(specular, "spotLight.specular");
	shader.setVec3f(direction, "spotLight.direction");
	glUniform1f(glGetUniformLocation(shader.getID(), "spotLight.constant"), constant);
	glUniform1f(glGetUniformLocation(shader.getID(), "spotLight.linear"), linear);
	glUniform1f(glGetUniformLocation(shader.getID(), "spotLight.quadratic"), quadratic);
	glUniform1f(glGetUniformLocation(shader.getID(), "spotLight.cutOff"), cutOff);
	glUniform1f(glGetUniformLocation(shader.getID(), "spotLight.outerCutOff"), outerCutOff);
}
