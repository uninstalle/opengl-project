#include "pattern.h"

#include <iostream>
#include <stdexcept>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "stb-master/stb_image.h"


float* AbsPattern::genVerticesArray(AbsPattern &pattern)
{
	float *array = new float[pattern.vertices.size()];
	for (int i = 0; i < pattern.vertices.size(); ++i)
	{
		array[i] = pattern.vertices.at(i);
	}
	return array;
}

unsigned* AbsPattern::genIndicesArray(AbsPattern& pattern)
{
	unsigned *array = new unsigned[pattern.indices.size()];
	for (int i = 0; i < pattern.indices.size(); ++i)
	{
		array[i] = pattern.indices.at(i);
	}
	return array;
}



void TrianglePattern::loadPattern()
{
	if (VAO_ID == 0)
		genVAO();
	if (VBO_ID == 0)
		genVBO();

	float *verticesArray = genVerticesArray(*this);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), verticesArray, GL_STATIC_DRAW);
	setVerticesArrayAttrib();
	delete[] verticesArray;

	if (!indices.empty())
	{
		if (EBO_ID == 0)
			genEBO();
		unsigned *indicesArray = genIndicesArray(*this);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*indices.size(), indicesArray, GL_STATIC_DRAW);
		delete[] indicesArray;
	}
}

void TrianglePattern::genVAO()
{
	glGenVertexArrays(1, &VAO_ID);
	glBindVertexArray(VAO_ID);
}

void TrianglePattern::genVBO()
{
	glGenBuffers(1, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
}

void TrianglePattern::genEBO()
{
	glGenBuffers(1, &EBO_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);
}

void TrianglePattern::drawArrays() const
{
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);
}

void TrianglePattern::drawElements() const
{
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
}



void TrianglePattern::setVerticesArrayAttrib() const
{
	//load position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	//load color data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void TrianglePattern::drawPattern()
{
	glBindVertexArray(VAO_ID);
	if (EBO_ID == 0)
		drawArrays();
	else
		drawElements();
}

void SpherePattern::genVerticesFromSphere(int deltaAlphaDivision, int deltaBetaDivision)
{
	//alpha is in x-z,beta is in x-y
	int alpha = 0, beta = 0;
	int halfRoundDivision = 180,
		RoundDivision = 360,
		DivisionsInXZ = halfRoundDivision / deltaAlphaDivision,
		DivisionsInXY = RoundDivision / deltaBetaDivision;

	vertices.reserve(DivisionsInXY*DivisionsInXZ);

	for (alpha = 0; alpha <= halfRoundDivision; alpha += deltaAlphaDivision)
	{
		float sina = std::sin(glm::radians(float(alpha)));
		float cosa = std::cos(glm::radians(float(alpha)));
		for (beta = 0; beta < RoundDivision; beta += deltaBetaDivision)
		{
			float b = glm::radians(float(beta));
			float x = sina * std::cos(b)*radius;
			float y = sina * std::sin(b)*radius;
			float z = cosa * radius;
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}
	indices.reserve(vertices.size() * 2);
	for (int i = 0; i < vertices.size() / 3 - DivisionsInXY; ++i)
	{
		if ((i + 1) % DivisionsInXY == 0) // i+1 is in next level
		{
			indices.push_back(i);
			indices.push_back(i + DivisionsInXY);
			indices.push_back(i + 1 - DivisionsInXY);
			indices.push_back(i + 1 - DivisionsInXY);
			indices.push_back(i + DivisionsInXY);
			indices.push_back(i + 1);
		}
		else {
			indices.push_back(i);
			indices.push_back(i + DivisionsInXY);
			indices.push_back(i + 1);
			indices.push_back(i + 1);
			indices.push_back(i + DivisionsInXY);
			indices.push_back(i + DivisionsInXY + 1);
		}
	}
}

void SpherePattern::setVerticesArrayAttrib() const
{
	//load position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
}


void SpherePattern::loadPattern()
{
	loadPattern(5, 5);
}

void SpherePattern::loadPattern(int alphaDivision, int betaDivision)
{
	genVerticesFromSphere(alphaDivision, betaDivision);
	TrianglePattern::loadPattern();
}


void SpherePattern::drawPattern()
{
	TrianglePattern::drawPattern();
}

void SpherePatternTextured::genVerticesFromSphere(int deltaAlphaDivision, int deltaBetaDivision)
{

	//alpha is in x-z,beta is in x-y
	int alpha = 0, beta = 0;
	int halfRoundDivision = 180,
		RoundDivision = 360,
		DivisionsInXZ = halfRoundDivision / deltaAlphaDivision,
		DivisionsInXY = RoundDivision / deltaBetaDivision + 1;

	vertices.reserve(DivisionsInXY*DivisionsInXZ);

	for (alpha = 0; alpha <= halfRoundDivision; alpha += deltaAlphaDivision)
	{
		float sina = std::sin(glm::radians(float(alpha)));
		float cosa = std::cos(glm::radians(float(alpha)));
		for (beta = 0; beta <= RoundDivision; beta += deltaBetaDivision)
		{
			float b = glm::radians(float(beta));
			float x = sina * std::cos(b)*radius;
			float y = sina * std::sin(b)*radius;
			float z = cosa * radius;
			float u = static_cast<float> (beta) / static_cast<float> (RoundDivision);
			float v = 1 - static_cast<float> (alpha) / static_cast<float> (halfRoundDivision);
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
			vertices.push_back(u);
			vertices.push_back(v);
		}
	}
	indices.reserve(vertices.size() / 5 * 6);
	for (int i = 0; i < vertices.size() / 5 - DivisionsInXY; ++i)
	{
		if ((i + 1) % DivisionsInXY == 0) // i+1 is in next level
		{
			indices.push_back(i);
			indices.push_back(i + DivisionsInXY);
			indices.push_back(i + 1 - DivisionsInXY);
			indices.push_back(i + 1 - DivisionsInXY);
			indices.push_back(i + DivisionsInXY);
			indices.push_back(i + 1);
		}
		else {
			indices.push_back(i);
			indices.push_back(i + DivisionsInXY);
			indices.push_back(i + 1);
			indices.push_back(i + 1);
			indices.push_back(i + DivisionsInXY);
			indices.push_back(i + DivisionsInXY + 1);
		}
	}
}

void SpherePatternTextured::setVerticesArrayAttrib() const
{
	//load position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	//load texture mapping data
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

void SpherePatternTextured::loadTexture(const char* path)
{
	glGenTextures(1, &Texture_ID);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		else
			throw std::runtime_error("nrChannels error.\n");

		glBindTexture(GL_TEXTURE_2D, Texture_ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture : " << path << "\n" << std::endl;
	}
	stbi_image_free(data);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture_ID);

}

unsigned loadTexture(const char* path)
{
	unsigned Texture_ID;
	glGenTextures(1, &Texture_ID);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		else
			throw std::runtime_error("nrChannels error.\n");

		glBindTexture(GL_TEXTURE_2D, Texture_ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture : " << path << "\n" << std::endl;
	}
	stbi_image_free(data);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture_ID);
	stbi_set_flip_vertically_on_load(false);

	return Texture_ID;
}

void SpherePatternTextured::drawPattern(ShaderProgram& shader)
{
	glUniform1i(glGetUniformLocation(shader.getID(), "material.texture_diffuse1"), 0);

	glBindTexture(GL_TEXTURE_2D, Texture_ID);
	glUniform1f(glGetUniformLocation(shader.getID(), "material.shininess"), 32.0f);
	glActiveTexture(0);

	glBindVertexArray(VAO_ID);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
	glBindVertexArray(0);
}

Mesh genSphereMesh(int deltaAlphaDivision, int deltaBetaDivision, const char* texturePath)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	std::vector<Texture> textures;
	float radius = 1;

	//alpha is in x-z,beta is in x-y
	int alpha = 0, beta = 0;
	int halfRoundDivision = 180,
		RoundDivision = 360,
		DivisionsInXZ = halfRoundDivision / deltaAlphaDivision,
		DivisionsInXY = RoundDivision / deltaBetaDivision + 1;

	vertices.reserve(DivisionsInXY*DivisionsInXZ);

	for (alpha = 0; alpha <= halfRoundDivision; alpha += deltaAlphaDivision)
	{
		float sina = std::sin(glm::radians(float(alpha)));
		float cosa = std::cos(glm::radians(float(alpha)));
		for (beta = 0; beta <= RoundDivision; beta += deltaBetaDivision)
		{
			float b = glm::radians(float(beta));
			float x = sina * std::cos(b)*radius;
			float y = sina * std::sin(b)*radius;
			float z = cosa * radius;
			float u = static_cast<float> (beta) / static_cast<float> (RoundDivision);
			float v = 1 - static_cast<float> (alpha) / static_cast<float> (halfRoundDivision);
			Vertex vertex{
			glm::vec3(x, y, z),
			glm::vec3(x, y, z),
			glm::vec2(u, v) };
			vertices.push_back(vertex);
		}
	}
	indices.reserve(vertices.size() * 6);
	for (int i = 0; i < vertices.size() - DivisionsInXY; ++i)
	{
		if ((i + 1) % DivisionsInXY == 0) // i+1 is in next level
		{
			indices.push_back(i);
			indices.push_back(i + DivisionsInXY);
			indices.push_back(i + 1 - DivisionsInXY);
			indices.push_back(i + 1 - DivisionsInXY);
			indices.push_back(i + DivisionsInXY);
			indices.push_back(i + 1);
		}
		else {
			indices.push_back(i);
			indices.push_back(i + DivisionsInXY);
			indices.push_back(i + 1);
			indices.push_back(i + 1);
			indices.push_back(i + DivisionsInXY);
			indices.push_back(i + DivisionsInXY + 1);
		}
	}
	if (texturePath)
	{
		Texture texture{ loadTexture(texturePath) ,Texture::DIFFUSE,texturePath };
		textures.push_back(texture);
	}
	return Mesh(std::move(vertices), std::move(indices), std::move(textures));
}