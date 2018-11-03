#include "shader.h"

#include <iostream>
#include <fstream>
#include <sstream>


#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vShaderCode = loadShaderCode(vertexPath);
	std::string fShaderCode = loadShaderCode(fragmentPath);

	compileShader(*this, vShaderCode.c_str(), fShaderCode.c_str());
}

std::string Shader::loadShaderCode(const char* shaderPath)
{
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(shaderPath);
	}
	catch (std::ifstream::failure &e)
	{
		std::cout << "Shader files read failed: " << shaderPath << std::endl;
	}
	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();
	std::string shaderString = shaderStream.str();
	return shaderString;
}

void Shader::compileShader(Shader &shader, const char* vShader, const char* fShader)
{

	int sResult;
	char infoLog[512];

	unsigned vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShader, nullptr);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &sResult);
	if (!sResult)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cout << "Vertex shader compilation failed.\n" << infoLog << std::endl;
	}

	unsigned fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShader, nullptr);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &sResult);
	if (!sResult)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cout << "Fragment shader compilation failed.\n" << infoLog << std::endl;
	}

	shader.ID = glCreateProgram();
	glAttachShader(shader.ID, vertex);
	glAttachShader(shader.ID, fragment);
	glLinkProgram(shader.ID);
	glGetProgramiv(shader.ID, GL_LINK_STATUS, &sResult);
	if (!sResult)
	{
		glGetProgramInfoLog(shader.ID, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}


void Shader::activate() const
{
	glUseProgram(ID);
}

//void Shader::setVec3f(const glm::vec3 &vec, const char *name)
//{
//	glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(vec));
//}
void Shader::setVec3f(glm::vec3 vec, const char *name)
{
	glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(vec));
}

//void Shader::setVec4f(const glm::vec4 &vec, const char *name)
//{
//	glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(vec));
//}

void Shader::setVec4f(glm::vec4 vec, const char *name)
{
	glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(vec));
}

//void Shader::setMat3f(const glm::mat3 &mat, const char *name)
//{
//	glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(mat));
//}

void Shader::setMat3f(glm::mat3 mat, const char *name)
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(mat));
}

//void Shader::setMat4f(const glm::mat4 &mat, const char *name)
//{
//	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(mat));
//}

void Shader::setMat4f(glm::mat4 mat, const char *name)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(mat));
}
