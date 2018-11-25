#include "shader.h"

#include <iostream>
#include <fstream>
#include <sstream>


#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* shaderPath,GLenum shaderType) :shaderType(shaderType)
{
	std::string shaderCode = loadShaderCode(shaderPath);

	try {
		compileShader(*this, shaderCode.c_str());
	}
	catch (std::runtime_error &e)
	{
		std::cout << "Shader compilation failed." << "Shader path:" << shaderPath << std::endl;
		std::cout << e.what() << std::endl;
	}
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

void Shader::compileShader(Shader &shader, const char* shaderCode)
{

	int sResult;
	char infoLog[512];

	unsigned shaderID = glCreateShader(shader.shaderType);
	glShaderSource(shaderID, 1, &shaderCode, nullptr);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &sResult);
	if (sResult != GL_TRUE)
	{
		glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
		throw std::runtime_error(infoLog);
	}
	shader.ID = shaderID;
	
}

void Shader::deleteShader()
{
	glDeleteShader(ID);
}


Shader::~Shader()
{
	deleteShader();
}

ShaderProgram::ShaderProgram()
{
	ID = glCreateProgram();
}

void ShaderProgram::attachShader(Shader *shader)
{
	if (std::find(attachedShader.begin(), attachedShader.end(), shader) != attachedShader.end())
		throw std::runtime_error("Attach shader failed.Shader has been attached.");
	else
	{
		glAttachShader(ID, shader->getID());
		attachedShader.push_back(shader);
	}
}

void ShaderProgram::detachShader(Shader *shader)
{
	auto targetShader = std::find(attachedShader.begin(), attachedShader.end(), shader);
	if (targetShader == attachedShader.end())
		throw std::runtime_error("Detach shader failed.Shader is not attached.");
	else
	{
		glDetachShader(ID, shader->getID());
		attachedShader.erase(targetShader);
	}
}

std::vector<Shader*> ShaderProgram::getAttachedShader() const
{
	return attachedShader;
}

void ShaderProgram::linkShaders()
{
	try
	{
		link();
	}
	catch (std::runtime_error &e)
	{
		std::cout << "Link shaders failed.Shader program ID:" << ID << std::endl;
		std::cout << e.what() << std::endl;
	}
}

void ShaderProgram::link()
{
	glLinkProgram(ID);
	int sResult;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &sResult);
	if (!sResult)
	{
		glGetProgramInfoLog(ID, 512, nullptr, infoLog);
		throw std::runtime_error(infoLog);
	}
}


void ShaderProgram::use() const
{
	glUseProgram(ID);
}


void ShaderProgram::setVec3f(const glm::vec3 vec, const char *name)
{
	glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(vec));
}

void ShaderProgram::setVec4f(const glm::vec4 vec, const char *name)
{
	glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(vec));
}


void ShaderProgram::setMat3f(const glm::mat3 mat, const char *name)
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::setMat4f(const glm::mat4 mat, const char *name)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(mat));
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(ID);
}
