#ifndef _MESH_H
#define _MESH_H
#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	enum TYPE { DIFFUSE = 1, SPECULAR = 2 };
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
	void setupMesh();
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> &&vertices, std::vector<unsigned> &&indices, std::vector<Texture> &&textures)
	:vertices(vertices),indices(indices),textures(textures)
	{
		setupMesh();
	}
	void draw(Shader &shader);

};


#endif
