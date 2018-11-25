#ifndef _SHADER_H
#define _SHADER_H
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Shader
{
private:
	unsigned ID;
	GLenum shaderType;
	static std::string loadShaderCode(const char* shaderPath);
	static void compileShader(Shader &shader, const char *shaderCode);
public:
	Shader(const char *shaderPath, GLenum shaderType);
	unsigned getID() const { return ID; }
	void deleteShader();
	~Shader();
};

class ShaderProgram
{
private:
	unsigned ID;
	std::vector<Shader*> attachedShader;
	void link();
public:
	ShaderProgram();
	unsigned getID() { return ID; }
	void attachShader(Shader *shader);
	void detachShader(Shader *shader);
	std::vector<Shader*> getAttachedShader() const;
	void linkShaders();
	void use() const;
	void setVec3f(const glm::vec3 vec, const char *name);
	void setVec4f(const glm::vec4 vec, const char *name);
	void setMat3f(const glm::mat3 vec, const char *name);
	void setMat4f(const glm::mat4 vec, const char *name);
	~ShaderProgram();
};


#endif