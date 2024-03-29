#ifndef _MESH_H
#define _MESH_H
#include <vector>
#include <glm/glm.hpp>
#include "shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec4 Tangent;
};

struct Texture
{
	enum TYPE { DIFFUSE = 1, SPECULAR = 2,NORMAL = 3 };
	unsigned ID;
	int type;
	std::string path;
};

class Mesh
{
private:
	unsigned VAO;
	unsigned VBO;
	unsigned EBO;
	bool isNormalTextured;
	void setupMesh();
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	std::vector<Texture> textures;
	Mesh() = default;
	Mesh(std::vector<Vertex> &&vertices, std::vector<unsigned> &&indices, std::vector<Texture> &&textures)
	:vertices(vertices),indices(indices),textures(textures)
	{
		setupMesh();
	}
	void draw(ShaderProgram &shader);
	bool hasNormalTex() const { return isNormalTextured; };

};

unsigned loadTexture(const char *filePath, const std::string directory);

#endif
