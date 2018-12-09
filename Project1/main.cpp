#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "posteffect.h"
#include "skybox.h"



constexpr float SCREEN_WIDTH = 1920, SCREEN_HEIGHT = 1080;
GLFWwindow *Window = nullptr;

float lastX = SCREEN_WIDTH / 2, lastY = SCREEN_HEIGHT / 2;

int isSpotLightOn = 0;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
CameraTransformMatrix CameraMatrix;

GLFWwindow* initGLFWWindow();
void initGLAD();

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void processCameraMovement(GLFWwindow *window);



int main()
{
	glfwInit();

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


	Model P51("resource/B-17E/B-17E.obj");

	Shader vertexShader("VertexShader.vert", GL_VERTEX_SHADER);
	Shader fragmentShader("FragmentShader.frag", GL_FRAGMENT_SHADER);
	Shader lightFragmentShader("LightFragmentShader.frag", GL_FRAGMENT_SHADER);

	ShaderProgram shaderProgram;
	shaderProgram.attachShader(vertexShader);
	shaderProgram.attachShader(fragmentShader);
	shaderProgram.linkShaders();

	ShaderProgram lightShader{ vertexShader,lightFragmentShader };
	lightShader.linkShaders();



	DirLight dirL(glm::vec3(0.0f, -1.0f, -1.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f));
	PointLight poiL(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f, 0.007f, 0.0002f);
	SpotLight spoL(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f, 0.09f, 0.032f, glm::vec3(0.0f, 0.0f, 0.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)));

	initializeScreenFrameBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	initializeSkybox();


	glClearColor(0.2f, 0.2f, 0.0f, 1.0f);



	while (!glfwWindowShouldClose(Window))
	{
		//inputting
		processInput(Window);
		synchronizeMovementSpeed();

		enableScreenFrameBuffer();

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		//glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		CameraMatrix.view = camera.GetViewMatrix();
		CameraMatrix.projection = glm::perspective(glm::radians(camera.getZoom()), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f);



		shaderProgram.use();
		shaderProgram.setVec3f(camera.getPosition(), "ViewPosition");
		shaderProgram.setMat4f(CameraMatrix.view, "view");
		shaderProgram.setMat4f(CameraMatrix.projection, "projection");
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		shaderProgram.setMat4f(model, "model");
		P51.draw(shaderProgram);

		shaderProgram.use();
		shaderProgram.setMat4f(CameraMatrix.view, "view");
		shaderProgram.setMat4f(CameraMatrix.projection, "projection");
		glm::vec3 pointPos = glm::vec3(cos(glfwGetTime()), 0.0f, sin(glfwGetTime()));
		poiL.setPosition(pointPos);
		spoL.setPosition(camera.getPosition());
		spoL.setDirection(camera.getFront());
		//dirL.apply(shaderProgram);
		poiL.apply(shaderProgram);
		spoL.apply(shaderProgram);
		glUniform1i(glGetUniformLocation(shaderProgram.getID(), "numOfSpotLights"), isSpotLightOn);

		lightShader.use();
		glm::mat4 lightModel(1.0f);
		lightModel = glm::translate(lightModel, pointPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.01f, 0.01f, 0.01f));
		lightShader.setMat4f(CameraMatrix.view, "view");
		lightShader.setMat4f(CameraMatrix.projection, "projection");
		lightShader.setMat4f(lightModel, "model");
		P51.draw(lightShader);


		drawSkybox(CameraMatrix.view, CameraMatrix.projection);


		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		disableScreenFrameBuffer();

		drawScreenFrameBuffer();

		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}



GLFWwindow* initGLFWWindow()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

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

	processCameraMovement(window);

	static bool isZpressed = false;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		isZpressed = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE && isZpressed)
	{
		isZpressed = false;
		isSpotLightOn = isSpotLightOn == 0 ? 1 : 0;
	}
}

void processCameraMovement(GLFWwindow *window)
{

	CameraMovement cameraMovement;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraMovement.Movement |= CameraMovement::FORWARD;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraMovement.Movement |= CameraMovement::BACKWARD;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraMovement.Movement |= CameraMovement::LEFT_SHIFT;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraMovement.Movement |= CameraMovement::RIGHT_SHIFT;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		cameraMovement.Movement |= CameraMovement::LEFT_ROLL;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		cameraMovement.Movement |= CameraMovement::RIGHT_ROLL;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		cameraMovement.Movement |= CameraMovement::UPWARD;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		cameraMovement.Movement |= CameraMovement::DOWNWARD;
	}

	camera.processKeyboardMovement(cameraMovement);
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
	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	camera.processMouseMovement(xOffset, yOffset);

}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.processMouseScroll(yOffset);
}
