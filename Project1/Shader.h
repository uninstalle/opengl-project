#ifndef _SHADER_H
#define _SHADER_H
#include <string>
#include <glm/glm.hpp>

class Shader
{
private:
	unsigned ID;
	static std::string loadShaderCode(const char* shaderPath);
	static void compileShader(Shader &shader, const char *vShader, const char *fShader);
public:
	Shader(const char *vertexShaderPath, const char *fragmentShaderChar);
	unsigned getID() { return ID; }
	void activate() const;
	void setVec3f(glm::vec3 &vec, const char *name);
	void setVec3f(glm::vec3 &&vec, const char *name);
	void setVec4f(glm::vec4 &vec, const char *name);
	void setVec4f(glm::vec4 &&vec, const char *name);
	void setMat3f(glm::mat3 &vec, const char *name);
	void setMat3f(glm::mat3 &&vec, const char *name);
	void setMat4f(glm::mat4 &vec, const char *name);
	void setMat4f(glm::mat4 &&vec, const char *name);
};



#endif