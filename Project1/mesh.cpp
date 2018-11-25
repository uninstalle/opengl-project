#include "mesh.h"
#include <glad/glad.h>
#include <string>

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

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Mesh::draw(ShaderProgram &shader)
{
	unsigned diffuse = 0;
	unsigned specular = 0;
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
		glUniform1i(glGetUniformLocation(shader.getID(), name.c_str()), i);

		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
	}
	glUniform1f(glGetUniformLocation(shader.getID(), "material.shininess"), 32.0f);
	glActiveTexture(0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
	glBindVertexArray(0);
}
