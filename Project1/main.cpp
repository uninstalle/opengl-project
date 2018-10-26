#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "pattern.h"
#include "light.h"
#include "model.h"

#include <stb-master/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


constexpr float SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;
GLFWwindow *Window = nullptr;

float deltaTime = 0, lastFrame = 0;
float lastX = 400, lastY = 300;

int isSpotLightOn = 0;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

GLFWwindow* initGLFWWindow();
void initGLAD();
void loadTexture(const GLchar *filePath, unsigned textureSlot);

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);


	try {
		Window = initGLFWWindow();
		initGLAD();
	}
	catch (std::runtime_error &e)
	{
		std::cout << e.what();
		glfwTerminate();
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glfwSetFramebufferSizeCallback(Window, framebufferSizeCallback);
	glfwSetCursorPosCallback(Window, mouseCallback);
	glfwSetScrollCallback(Window, scrollCallback);


	float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  1.0f,  2.0f),
  glm::vec3(2.0f,  5.0f, -15.0f)
	};

	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);


	TrianglePatternTextured box;
	box.loadPattern(vertices, sizeof(vertices));

	Shader shaderProgram("VertexShader.glsl", "FragmentShader.glsl");

	loadTexture("resource/YJSP.jpg", GL_TEXTURE0);
	loadTexture("resource/YJSP_spe.jpg", GL_TEXTURE1);

	shaderProgram.activate();
	glUniform1i(glGetUniformLocation(shaderProgram.getID(), "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram.getID(), "material.specular"), 1);
	glUniform1f(glGetUniformLocation(shaderProgram.getID(), "material.shininess"), 32.0f);

	DirLight dirL(glm::vec3(0.0f, -1.0f, -1.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f));
	PointLight poiL(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f, 0.09f, 0.032f);
	SpotLight spoL(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f, 0.09f, 0.032f, glm::vec3(0.0f, 0.0f, 0.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)));


	TrianglePatternTextured lightSource;
	lightSource.loadPattern(vertices, sizeof(vertices));

	Shader lightShaderProgram("VertexShader.glsl", "LightFragmentShader.glsl");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(Window))
	{
		//inputting
		processInput(Window);
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightShaderProgram.activate();

		lightShaderProgram.setMat4f(view, "view");
		lightShaderProgram.setMat4f(projection, "projection");
		glm::mat4 lightModel(1.0f);
		glm::vec3 lightTrans(cos(glfwGetTime()), 0.0f, sin(glfwGetTime()));
		lightModel = glm::translate(lightModel, lightTrans);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f, 0.2f, 0.2f));
		lightShaderProgram.setMat4f(lightModel, "model");
		lightSource.drawPattern();

		shaderProgram.activate();
		shaderProgram.setMat4f(view, "view");
		shaderProgram.setMat4f(projection, "projection");
		shaderProgram.setVec3f(camera.Position, "viewPos");
		poiL.setPosition(lightTrans);
		spoL.setPosition(camera.Position);
		spoL.setDirection(camera.Front);
		dirL.apply(shaderProgram);
		poiL.apply(shaderProgram);
		spoL.apply(shaderProgram);
		glUniform1i(glGetUniformLocation(shaderProgram.getID(), "isSpotLightOn"),  isSpotLightOn);


		for (int i = 0; i != 2; ++i)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			shaderProgram.setMat4f(model, "model");
			box.drawPattern();
		}



		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}



GLFWwindow* initGLFWWindow()
{
	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "?", nullptr, nullptr);
	if (window == nullptr)
	{
		throw std::runtime_error("Failed to initialize GLFW window.\n");
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return window;
}

void initGLAD()
{
	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		throw std::runtime_error("Failed to initialize GLAD.\n");
	}
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	CameraMovement direction;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		direction = FORWARD;
		camera.ProcessKeyboard(direction, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		direction = BACKWARD;
		camera.ProcessKeyboard(direction, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		direction = LEFT;
		camera.ProcessKeyboard(direction, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		direction = RIGHT;
		camera.ProcessKeyboard(direction, deltaTime);
	}

	static bool isZpressed = false;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		isZpressed = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE&&isZpressed)
	{
		isZpressed = false;
		isSpotLightOn = isSpotLightOn == 0 ? 1 : 0;
	}
}
void loadTexture(const GLchar *filePath, unsigned textureSlot)
{

	unsigned texture;
	glGenTextures(1, &texture);

	glActiveTexture(textureSlot);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(filePath, &width, &height, &nrChannels, 0);

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
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture : " << filePath << "\n" << std::endl;
	}
	stbi_image_free(data);
}


void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
	static bool isFirst = true;
	if (isFirst)
	{
		isFirst = false;
		lastX = xpos;
		lastY = ypos;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);

}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}