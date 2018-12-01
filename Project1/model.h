#ifndef _MODEL_H
#define _MODEL_H
#include "mesh.h"
#include <assimp/scene.h>


class Model
{
private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> loadedTextures;
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType textureType, int type);
public:
	Model(const char *path);
	void draw(ShaderProgram &shader);
};


#endif