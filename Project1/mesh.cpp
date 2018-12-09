#include "mesh.h"
#include <glad/glad.h>
#include <string>
#include <stdexcept>
#include <stb-master/stb_image.h>

static unsigned DefaultWhiteTexture;
static bool isMeshInitialized = false;

void initializeMesh()
{
	DefaultWhiteTexture = loadTexture("white20.png", "resource");
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Tangent)));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, BiTangent)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);
}

void Mesh::draw(ShaderProgram &shader)
{
	unsigned diffuse = 0;
	unsigned specular = 0;
	unsigned normal = 0;
	for (int i = 0; i < textures.size(); ++i)
	{
		std::string name = "material.";
		glActiveTexture(GL_TEXTURE0 + i);
		if (textures[i].type == Texture::DIFFUSE)
		{
			name.append("texture_diffuse");
			name.append(std::to_string(++diffuse));
		}
		else if (textures[i].type == Texture::SPECULAR)
		{
			name.append("texture_specular");
			name.append(std::to_string(++specular));
		}
		else if (textures[i].type == Texture::NORMAL)
		{
			name.append("texture_normal");
			name.append(std::to_string(++normal));
		}
		glUniform1i(glGetUniformLocation(shader.getID(), name.c_str()), i);

		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
	}
	if (textures.empty())
	{
		if (!isMeshInitialized)
			initializeMesh();
		glBindTexture(GL_TEXTURE_2D, DefaultWhiteTexture);
	}
	glUniform1f(glGetUniformLocation(shader.getID(), "material.shininess"), 32.0f);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(0);
}

unsigned loadTexture(const char *filePath, const std::string directory)
{
	std::string fileName(filePath);
	fileName = directory + '/' + fileName;

	unsigned texture;
	glGenTextures(1, &texture);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);

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

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		throw std::runtime_error("Failed to load texture" + fileName);
	}
	stbi_image_free(data);

	return texture;
}
