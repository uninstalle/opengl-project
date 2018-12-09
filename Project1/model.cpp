#include "model.h"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

Model::Model(const char* path)
{
	try {
		loadModel(path);
	}
	catch (std::runtime_error &e)
	{
		std::cout << "Error in loading model." << std::endl;
		std::cout << e.what() << std::endl;
	}
}


void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		throw std::runtime_error(importer.GetErrorString());
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (int i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	std::vector<Texture> textures;
	for (int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		if (mesh->HasNormals())
			vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		else
			vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);

		if (mesh->HasTextureCoords(0))
			vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		if (mesh->HasTangentsAndBitangents())
		{
			vertex.Tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			vertex.BiTangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}
		else
			vertex.Tangent = vertex.BiTangent = glm::vec3(0.0f, 0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	for (int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuse_map = loadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::DIFFUSE);
		textures.insert(textures.end(), diffuse_map.begin(), diffuse_map.end());
		std::vector<Texture> specular_map = loadMaterialTextures(material, aiTextureType_SPECULAR, Texture::SPECULAR);
		textures.insert(textures.end(), specular_map.begin(), specular_map.end());
		std::vector<Texture> normal_map = loadMaterialTextures(material, aiTextureType_HEIGHT, Texture::NORMAL);
		textures.insert(textures.end(), normal_map.begin(), normal_map.end());
	}
	return Mesh(std::move(vertices), std::move(indices), std::move(textures));
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType textureType, int type)
{
	std::vector<Texture> textures;
	for (int i = 0; i < material->GetTextureCount(textureType); ++i)
	{
		aiString str;
		material->GetTexture(textureType, i, &str);
		bool isLoaded = false;
		for (int j = 0; j < loadedTextures.size(); ++j)
		{
			if (loadedTextures[j].path == str.C_Str())
			{
				textures.push_back(loadedTextures[j]);
				isLoaded = true;
				break;
			}
		}
		if (!isLoaded)
		{
			Texture texture;
			texture.ID = loadTexture(str.C_Str(), directory);
			texture.type = type;
			texture.path = str.C_Str();
			textures.push_back(texture);
			loadedTextures.push_back(texture);
		}
	}
	return textures;
}

void Model::draw(ShaderProgram &shader)
{
	for (auto &i : meshes)
	{
		i.draw(shader);
	}
}
