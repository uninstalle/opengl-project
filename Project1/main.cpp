#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "camera.h"
#include "pattern.h"
#include "light.h"
#include "model.h"



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



Mesh mercury;
Mesh venus;
Mesh earth;
Mesh mars;
Mesh jupiter;
Mesh saturn;
Mesh uranus;
Mesh neptune;
Mesh sun;

void initializeStars();
void updateStarsMovement(ShaderProgram &shader);

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


	initializeStars();

	Shader vertexShader("VertexShader.vert",GL_VERTEX_SHADER);
	Shader fragmentShader("FragmentShader.frag",GL_FRAGMENT_SHADER);
	Shader lightFragmentShader("LightFragmentShader.frag",GL_FRAGMENT_SHADER);

	ShaderProgram shaderProgram;
	shaderProgram.attachShader(vertexShader);
	shaderProgram.attachShader(fragmentShader);
	shaderProgram.linkShaders();

	ShaderProgram lightShader{ vertexShader,lightFragmentShader };
	lightShader.linkShaders();



	DirLight dirL(glm::vec3(0.0f, -1.0f, -1.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f));
	PointLight poiL(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f, 0.007f, 0.0002f);
	SpotLight spoL(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f, 0.09f, 0.032f, glm::vec3(0.0f, 0.0f, 0.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)));




	glClearColor(0.2f, 0.2f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	

	while (!glfwWindowShouldClose(Window))
	{
		//inputting
		processInput(Window);
		synchronizeMovementSpeed();

		CameraMatrix.view = camera.GetViewMatrix();
		CameraMatrix.projection = glm::perspective(glm::radians(camera.getZoom()), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightShader.use();

		lightShader.setMat4f(CameraMatrix.view, "view");
		lightShader.setMat4f(CameraMatrix.projection, "projection");
		lightShader.setVec3f(glm::vec3(1.0f, 1.0f, 1.0f), "lightColor");
		glm::mat4 lightModel(1.0f);
		lightModel = glm::scale(lightModel, glm::vec3(0.696f * 2, 0.696f * 2, 0.696f * 2));
		lightShader.setMat4f(lightModel, "model");
		sun.draw(lightShader);

		shaderProgram.use();
		shaderProgram.setMat4f(CameraMatrix.view, "view");
		shaderProgram.setMat4f(CameraMatrix.projection, "projection");
		shaderProgram.setVec3f(camera.getPosition(), "viewPos");
		poiL.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		spoL.setPosition(camera.getPosition());
		spoL.setDirection(camera.getFront());
		//dirL.apply(shaderProgram);
		poiL.apply(shaderProgram);
		spoL.apply(shaderProgram);
		glUniform1i(glGetUniformLocation(shaderProgram.getID(), "isSpotLightOn"), isSpotLightOn);

		updateStarsMovement(shaderProgram);

		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}



GLFWwindow* initGLFWWindow()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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

void initializeStars()
{
	mercury = genSphereMesh(10, 10, "resource/mercury.jpg");
	venus = genSphereMesh(10, 10, "resource/venus.jpg");
	earth = genSphereMesh(10, 10, "resource/earth.jpg");
	mars = genSphereMesh(10, 10, "resource/mars.jpg");
	jupiter = genSphereMesh(10, 10, "resource/jupiter.jpg");
	saturn = genSphereMesh(10, 10, "resource/saturn.jpg");
	uranus = genSphereMesh(10, 10, "resource/uranus.jpg");
	neptune = genSphereMesh(10, 10, "resource/neptune.jpg");
	sun = genSphereMesh(5, 5);
}

void updateStarsMovement(ShaderProgram &shader)
{

	glm::mat4 model(1.0f);
	glm::vec3 movement(0.3871 * 1.496 * 2.5, 0, 0.3871 * 1.496 * 2.5);
	model = glm::rotate(model, (float)glm::radians(4.15*glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, movement);
	model = glm::scale(model, glm::vec3(0.0244f, 0.0244f, 0.0244f));
	model = glm::rotate(model, (float)glm::radians(0.017*glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4f(model, "model");
	mercury.draw(shader);

	model = glm::mat4(1.0f);
	movement = glm::vec3(0.7233 * 1.496 * 2.5, 0, 0.7233 * 1.496 * 2.5);
	model = glm::rotate(model, (float)glm::radians(1.62*glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, movement);
	model = glm::scale(model, glm::vec3(0.06052f, 0.06052f, 0.06052f));
	model = glm::rotate(model, (float)glm::radians(0.004*glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4f(model, "model");
	venus.draw(shader);

	model = glm::mat4(1.0f);
	movement = glm::vec3(1.496 * 2.5, 0, 1.496 * 2.5);
	model = glm::rotate(model, (float)glm::radians(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, movement);
	model = glm::scale(model, glm::vec3(0.06378f, 0.06378f, 0.06378f));
	model = glm::rotate(model, (float)glm::radians(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4f(model, "model");
	earth.draw(shader);

	model = glm::mat4(1.0f);
	movement = glm::vec3(1.5237 * 1.496 * 2.5, 0, 1.5237 * 1.496 * 2.5);
	model = glm::rotate(model, (float)glm::radians(0.53*glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, movement);
	model = glm::scale(model, glm::vec3(0.03397f, 0.03397f, 0.03397f));
	model = glm::rotate(model, (float)glm::radians(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4f(model, "model");
	mars.draw(shader);

	model = glm::mat4(1.0f);
	movement = glm::vec3(5.2026 * 1.496 * 2.5, 0, 5.2026 * 1.496 * 2.5);
	model = glm::rotate(model, (float)glm::radians(0.084*glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, movement);
	model = glm::scale(model, glm::vec3(0.71492f, 0.71492f, 0.71492f));
	model = glm::rotate(model, (float)glm::radians(2.5*glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4f(model, "model");
	jupiter.draw(shader);

	model = glm::mat4(1.0f);
	movement = glm::vec3(9.5549 * 1.496 * 2.5, 0, 9.5549 * 1.496 * 2.5);
	model = glm::rotate(model, (float)glm::radians(0.034*glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, movement);
	model = glm::scale(model, glm::vec3(0.60268f, 0.60268f, 0.60268f));
	model = glm::rotate(model, (float)glm::radians(2.4*glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4f(model, "model");
	saturn.draw(shader);

	model = glm::mat4(1.0f);
	movement = glm::vec3(19.2184 * 1.496 * 2.5, 0, 19.2184 * 1.496 * 2.5);
	model = glm::rotate(model, (float)glm::radians(0.012*glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, movement);
	model = glm::scale(model, glm::vec3(0.25559f, 0.25559f, 0.25559f));
	model = glm::rotate(model, (float)glm::radians(1.4*glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4f(model, "model");
	uranus.draw(shader);

	model = glm::mat4(1.0f);
	movement = glm::vec3(30.1104 * 1.496 * 2.5, 0, 30.1104 * 1.496 * 2.5);
	model = glm::rotate(model, (float)glm::radians(0.006*glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, movement);
	model = glm::scale(model, glm::vec3(0.24764f, 0.24764f, 0.24764f));
	model = glm::rotate(model, (float)glm::radians(1.5*glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4f(model, "model");
	neptune.draw(shader);

	model = glm::mat4(1.0f);
	movement = glm::vec3(1.496 * 2.5, 0, 1.496 * 2.5);
	model = glm::rotate(model, (float)glm::radians(2*glfwGetTime()), glm::vec3(0.0f, 1.0f, 1.0f));
	model = glm::translate(model, movement);
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	model = glm::rotate(model, (float)glm::radians(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4f(model, "model");
	earth.draw(shader);

	model = glm::mat4(1.0f);
	movement = glm::vec3(1.496 * 2.5, 0, 1.496 * 2.5);
	model = glm::rotate(model, (float)glm::radians(2*glfwGetTime()), glm::vec3(0.0f, 1.0f, 1.0f));
	glm::vec3 movement2(glm::cos(glm::radians(20 * glfwGetTime())), 0, glm::sin(glm::radians(20 * glfwGetTime())));
	model = glm::translate(model, movement2);
	model = glm::translate(model, movement);
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	model = glm::rotate(model, (float)glm::radians(10*glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4f(model, "model");
	earth.draw(shader);
}