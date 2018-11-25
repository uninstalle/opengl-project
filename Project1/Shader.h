#ifndef _SHADER_H
#define _SHADER_H
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

// When initialize a shader, users have to define the type of this shader.
// GLenum shaderType can be GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_TESS_CONTROL_SHADER
// GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER or GL_COMPUTE_SHADER.
class Shader
{
private:
	unsigned ID;
	GLenum shaderType;
	static std::string loadShaderCode(const char* shaderPath);
	static void compileShader(Shader &shader, const char *shaderCode);
public:
	Shader(const char *shaderPath, GLenum shaderType);
	Shader(const Shader &shader) = default;
	Shader(Shader &&shader) noexcept { ID = shader.ID; shaderType = shader.shaderType; shader.ID = 0; }
	unsigned getID() const { return ID; }
	void deleteShader();
	~Shader();
};

// Before use a shader program, users need to attach proper shaders, and link
// after all shaders attached. If shaders attached or detached after the program
// linked, the program need to link again to apply these changes.
class ShaderProgram
{
	using ShaderID = unsigned;
private:
	unsigned ID;
	std::vector<ShaderID> attachedShader;
	void link();
public:
	ShaderProgram();
	ShaderProgram(std::initializer_list<Shader> shaderIL);
	unsigned getID() const { return ID; }
	void attachShader(Shader shader);
	void detachShader(Shader shader);
	std::vector<ShaderID> getAttachedShader() const;
	void linkShaders();
	void use() const;
	void setVec3f(const glm::vec3 vec, const char *name);
	void setVec4f(const glm::vec4 vec, const char *name);
	void setMat3f(const glm::mat3 vec, const char *name);
	void setMat4f(const glm::mat4 vec, const char *name);
	void setUBO(unsigned UBO, int size, const char *name, int offset = 0);
	~ShaderProgram();
};


#endif