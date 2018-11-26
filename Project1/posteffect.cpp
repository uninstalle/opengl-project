#include "posteffect.h"
#include <iostream>
#include "glad/glad.h"
#include "shader.h"

static unsigned ScreenFrameBuffer;
static unsigned ScreenColorTexture;
static unsigned ScreenFrameBufferVAO;
static ShaderProgram *PostEffectShader;

enum PostEffect
{
	INVERSION = 1,
	GRAYSCALE = 2,
	SHARPEN = 4,
	BLUR = 8,
	EDGE_DETECTION = 16
};


static void initializeVAOForScreenFrameBuffer()
{
	unsigned VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	float screen[]{
		-1.0f,1.0f, 0.0f,1.0f,
		-1.0f,-1.0f, 0.0f,0.0f,
		1.0f,-1.0f, 1.0f,0.0f,
		1.0f,-1.0f, 1.0f,0.0f,
		1.0f,1.0f, 1.0f,1.0f,
		-1.0f,1.0f, 0.0f,1.0f
	};
	unsigned VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screen), screen, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	ScreenFrameBufferVAO = VAO;
}

static void initializePostEffectShader()
{
	Shader vShader("PostEffectVertexShader.vert", GL_VERTEX_SHADER);
	Shader fShader("PostEffectFragmentShader.frag", GL_FRAGMENT_SHADER);
	PostEffectShader = new ShaderProgram{ vShader, fShader };
	PostEffectShader->linkShaders();
}

static unsigned genScreenFrameBuffer()
{
	unsigned frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	return frameBuffer;
}

static unsigned genColorTextureBuffer(int screenWidth, int screenHeight)
{
	unsigned texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texColorBuffer;
}

static unsigned genRenderBuffer(int screenWidth, int screenHeight)
{
	unsigned rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	return rbo;
}

void initializeScreenFrameBuffer(int screenWidth, int screenHeight)
{
	try {


		initializePostEffectShader();

		ScreenFrameBuffer = genScreenFrameBuffer();
		glBindFramebuffer(GL_FRAMEBUFFER, ScreenFrameBuffer);

		ScreenColorTexture = genColorTextureBuffer(screenWidth, screenHeight);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ScreenColorTexture, 0);

		unsigned rbo = genRenderBuffer(screenWidth, screenHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			throw std::runtime_error("Framebuffer not complete.");

		initializeVAOForScreenFrameBuffer();
	}
	catch (std::runtime_error &e)
	{
		std::cout << e.what() << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void enableScreenFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ScreenFrameBuffer);
}

void disableScreenFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void drawScreenFrameBuffer(unsigned postEffect)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	PostEffectShader->use();
	glBindVertexArray(ScreenFrameBufferVAO);
	glBindTexture(GL_TEXTURE_2D, ScreenColorTexture);
	glUniform1i(glGetUniformLocation(PostEffectShader->getID(), "ProcessMode"), postEffect);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}