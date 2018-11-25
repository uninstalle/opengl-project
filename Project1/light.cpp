#include "light.h"
#include <glad/glad.h>

void DirLight::apply(ShaderProgram& shaderProgram)
{
	shaderProgram.setVec3f(position, "dirLight.position");
	shaderProgram.setVec3f(ambient, "dirLight.ambient");
	shaderProgram.setVec3f(diffuse, "dirLight.diffuse");
	shaderProgram.setVec3f(specular, "dirLight.specular");
}

int PointLight::PointLightCount = 0;

void PointLight::apply(ShaderProgram& shaderProgram)
{
	glUniform1i(glGetUniformLocation(shaderProgram.getID(), "PointLightNum"), PointLightCount);
	char lightIndex = '0' + PointLightCount - 1;
	std::string plightName("pointLight[0].");
	plightName.at(plightName.find('0')) = lightIndex;
	shaderProgram.setVec3f(position, std::string(plightName + "position").c_str());
	shaderProgram.setVec3f(ambient, std::string(plightName + "ambient").c_str());
	shaderProgram.setVec3f(diffuse, std::string(plightName + "diffuse").c_str());
	shaderProgram.setVec3f(specular, std::string(plightName + "specular").c_str());
	glUniform1f(glGetUniformLocation(shaderProgram.getID(), std::string(plightName + "constant").c_str()), constant);
	glUniform1f(glGetUniformLocation(shaderProgram.getID(), std::string(plightName + "linear").c_str()), linear);
	glUniform1f(glGetUniformLocation(shaderProgram.getID(), std::string(plightName + "quadratic").c_str()), quadratic);
}

void SpotLight::apply(ShaderProgram& shaderProgram)
{
	shaderProgram.setVec3f(position, "spotLight.position");
	shaderProgram.setVec3f(ambient, "spotLight.ambient");
	shaderProgram.setVec3f(diffuse, "spotLight.diffuse");
	shaderProgram.setVec3f(specular, "spotLight.specular");
	shaderProgram.setVec3f(direction, "spotLight.direction");
	glUniform1f(glGetUniformLocation(shaderProgram.getID(), "spotLight.constant"), constant);
	glUniform1f(glGetUniformLocation(shaderProgram.getID(), "spotLight.linear"), linear);
	glUniform1f(glGetUniformLocation(shaderProgram.getID(), "spotLight.quadratic"), quadratic);
	glUniform1f(glGetUniformLocation(shaderProgram.getID(), "spotLight.cutOff"), cutOff);
	glUniform1f(glGetUniformLocation(shaderProgram.getID(), "spotLight.outerCutOff"), outerCutOff);
}
