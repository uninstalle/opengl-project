#ifndef _LIGHT_H
#define _LIGHT_H
#include <glm/glm.hpp>
#include "Shader.h"

class AbsLight
{
protected:
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

public:
	AbsLight() = default;
	AbsLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
		:position(position), ambient(ambient), diffuse(diffuse), specular(specular) {}
	virtual void setPosition(glm::vec3 position) { this->position = position; }
	virtual void setAmbient(glm::vec3 ambient) { this->ambient = ambient; }
	virtual void setDiffuse(glm::vec3 diffuse) { this->diffuse = diffuse; }
	virtual void setSpecular(glm::vec3 specular) { this->specular = specular; }
	virtual void apply(Shader &shader) = 0;
	virtual ~AbsLight() = default;
};

class DirLight :public AbsLight
{
public:
	DirLight() = default;
	DirLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
		:AbsLight(position, ambient, diffuse, specular) {}
	void apply(Shader &shader) override;
	~DirLight() = default;
};

class PointLight :public AbsLight
{
private:
	float constant;
	float linear;
	float quadratic;
	static int PointLightCount;
public:
	PointLight() { PointLightCount++; };
	PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
		:AbsLight(position, ambient, diffuse, specular), constant(constant), linear(linear), quadratic(quadratic)
	{
		PointLightCount++;
	}
	void setConstant(float constant) { this->constant = constant; }
	void setLinear(float linear) { this->linear = linear; }
	void setQuadratic(float quadratic) { this->quadratic = quadratic; }
	void apply(Shader& shader) override;
	~PointLight() { PointLightCount--; };
};

class SpotLight :public AbsLight
{
private:
	float constant;
	float linear;
	float quadratic;
	glm::vec3 direction;
	float cutOff;
	float outerCutOff;
public:
	SpotLight() = default;
	SpotLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		float constant, float linear, float quadratic, glm::vec3 direction, float cutoff,float outercutoff)
		: AbsLight(position, ambient, diffuse, specular), constant(constant),
		linear(linear), quadratic(quadratic), direction(direction), cutOff(cutoff),outerCutOff(outercutoff) {}
	void setConstant(float constant) { this->constant = constant; }
	void setLinear(float linear) { this->linear = linear; }
	void setQuadratic(float quadratic) { this->quadratic = quadratic; }
	void setDirection(glm::vec3 direction) { this->direction = direction; }
	void setCutOff(float cutoff) { this->cutOff = cutoff; }
	void setOuterCutOff(float outercutoff) { this->outerCutOff = outercutoff; }
	void apply(Shader& shader) override;
	~SpotLight() = default;
};

#endif
