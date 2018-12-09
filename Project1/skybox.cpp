#include "skybox.h"
#include "shader.h"
#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <stb-master/stb_image.h>

static ShaderProgram *SkyboxShader;
static unsigned SkyboxVAO;
static unsigned SkyboxTexture;

static void initializeSkyboxShader()
{
	Shader vShader("CubeMap.vert", GL_VERTEX_SHADER);
	Shader fShader("CubeMap.frag", GL_FRAGMENT_SHADER);
	SkyboxShader = new ShaderProgram{ vShader,fShader };
	SkyboxShader->linkShaders();
}


static unsigned loadCubeMap(const std::vector<std::string> paths)
{
	unsigned textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < paths.size(); i++)
	{
		unsigned char *data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << paths[i] << std::endl;
		}
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

static unsigned initializeSkyboxVAO()
{
	unsigned VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	float skyboxVertices[] = {        
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	return VAO;
}

void initializeSkybox()
{
	std::vector<std::string> paths
	{
		"resource/skybox/right.tga",
			"resource/skybox/left.tga",
			"resource/skybox/top.tga",
			"resource/skybox/bottom.tga",
			"resource/skybox/front.tga",
			"resource/skybox/back.tga"
	};

	SkyboxTexture = loadCubeMap(paths);
	SkyboxVAO = initializeSkyboxVAO();

	initializeSkyboxShader();
}

void drawSkybox(glm::mat4 view,glm::mat4 projection)
{
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	SkyboxShader->use();
	SkyboxShader->setMat4f(view, "view");
	SkyboxShader->setMat4f(projection, "projection");
	glBindVertexArray(SkyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}