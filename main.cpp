
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.hpp"
#include "Texture.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Primitive.hpp"
#include <vector>
#include <cmath>
#include "Inputs.hpp"
#include "Material.hpp"
#include "Constants.hpp"
#include "Light.hpp"

using namespace std;

static constexpr int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

//time
static float deltaTime;

//camera variables

static constexpr float TURN_SENSITIVITY = 0.1f, TRANSLATION_SENSITIVITY = 2.0f;
static bool shiftFunctionsEnabled = false, controlFunctionsEnabled = false;
static double oldXPos, oldYPos;
static float yaw = -90.0f, pitch = 0;
static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

static void mouse_callback(GLFWwindow* window, double xpos,double ypos){
	if (!controlFunctionsEnabled) {
		
		yaw += (xpos - oldXPos) * TURN_SENSITIVITY;
		pitch = max(min( pitch + ((oldYPos - ypos) * TURN_SENSITIVITY), 89.9), -89.9);

		cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront.y = sin(glm::radians(pitch));
		cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(cameraFront);
	}
	oldXPos = xpos;
	oldYPos = ypos;
}
static void initEditorInputs(GLFWwindow* window) {
	Inputs::init(window);
	Inputs::addKeyCallback({ GLFW_KEY_ESCAPE ,GLFW_PRESS }, [window]() {glfwSetWindowShouldClose(window, true); });
	Inputs::addKeyCallback({ GLFW_KEY_LEFT_SHIFT ,GLFW_PRESS }, []() {shiftFunctionsEnabled = true; });
	Inputs::addKeyCallback({ GLFW_KEY_LEFT_SHIFT ,GLFW_RELEASE }, []() {shiftFunctionsEnabled = false; });
	Inputs::addKeyCallback({ GLFW_KEY_LEFT_CONTROL ,GLFW_PRESS }, []() {controlFunctionsEnabled = true;  });
	Inputs::addKeyCallback({ GLFW_KEY_LEFT_CONTROL ,GLFW_RELEASE }, []() {controlFunctionsEnabled = false;  });

	Inputs::addContinuousKeyCallback({ GLFW_KEY_W ,GLFW_PRESS }, []() {cameraPos += deltaTime * TRANSLATION_SENSITIVITY * cameraFront; });
	Inputs::addContinuousKeyCallback({ GLFW_KEY_A ,GLFW_PRESS }, []() {cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * TRANSLATION_SENSITIVITY * deltaTime; });
	Inputs::addContinuousKeyCallback({ GLFW_KEY_S ,GLFW_PRESS }, []() {cameraPos -= deltaTime * TRANSLATION_SENSITIVITY * cameraFront; });
	Inputs::addContinuousKeyCallback({ GLFW_KEY_D ,GLFW_PRESS }, []() {cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * TRANSLATION_SENSITIVITY * deltaTime; });
	Inputs::addContinuousKeyCallback({ GLFW_KEY_E ,GLFW_PRESS }, []() {cameraPos += deltaTime * TRANSLATION_SENSITIVITY * cameraUp; });
	Inputs::addContinuousKeyCallback({ GLFW_KEY_Q ,GLFW_PRESS }, []() {cameraPos -= deltaTime * TRANSLATION_SENSITIVITY * cameraUp; });

}
static GLFWwindow* initOpenGlLibraries() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGlGameEngine", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create glfw window" << endl;
		throw runtime_error{"GLFW init error"};

	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGL()) {
		cout << "Failed to load GLAD" << endl;
		throw runtime_error{ "GLAD init error" };
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);//display rendering on the whole window

	glfwGetCursorPos(window, &oldXPos, &oldYPos);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glEnable(GL_DEPTH_TEST);
	return window;
}

int main() {

	//setup
	GLFWwindow* window = initOpenGlLibraries();
	initEditorInputs(window);

	//variables
	glm::vec3 lightPosition = { -2.0f, 0.0f, 1.65f };
	glm::vec3 cubePosition = { 0.0f, 0.0f, 0.0f };
	glm::vec3 secondCubePosition = { 0.0f, 2.0f, 0.0f };
	glm::vec3 secondCubeChildPosition = { 0.0f, 1.0f, 0.0f };

	float currentTime, previousTime;


	//shaders
	Shader shader("shader.vs", "shader.fs");



	//GameObjects setup
	Cube cube = Cube(&shader, { cubePosition,{0, glm::pi<float>() / 3,0} });
	Cube secondCube = Cube(&shader, { secondCubePosition });
	Cube secondCubeChild = Cube(&shader, { secondCubeChildPosition },0.1f);
	secondCubeChild.setParent(&secondCube);

	//Cube lightCube = Cube(& lightShader, {lightPosition}, 0.1f);
	//Cube lightCube = Cube(& lightShader, {lightPosition}, 0.1f);

	//textures
	//Texture textureA("container.jpg", &shader);
	//cube.linkChild(&textureA);
	//secondCubeChild.linkChild(&textureA);
	//Texture textureB("coolGuy.png", &shader);
	//secondCube.linkChild(&textureB);

	//other stuff >:)
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	shader.useThenSetMat4f("uProjection", &projection);

	LightColors colors;
	colors.ambient = { 0.05f, 0.05f, 0.05f };
	colors.diffuse = { 0.2f, 0.2f, 0.2f };
	colors.specular = { 0.5f, 0.5f, 0.5f };

	LightColors colorsB;
	colorsB.ambient = { 0.02f, 0.02f, 0.02f };
	colorsB.diffuse = { 0.5f,0.1f,0.1f };
	colorsB.specular = { 1.0f, 0.0f, 0.0f };

	DirectionalLight directionalLight(&shader, { -1,-1,0 }, colors);
	directionalLight.addToShader();
	PointLight pointLight(&shader, { lightPosition }, { 0.09f,0.032f }, colorsB);
	pointLight.addToShader();

	//lightCube.linkChild(&pointLight);

	LightShader::lightShader().useThenSetMat4f("uProjection", &projection);

	Material mat("container2.png", "container2_specular.png", &shader, {1.0f, 0.5f, 0.31f}, {1.0f, 0.5f, 0.31f}, {0.5f, 0.5f, 0.5f}, 100.0f);
	Material mat2("coolGuy.png", "coolGuy.png", &shader, {1.0f, 0.5f, 0.31f}, {1.0f, 0.5f, 0.31f}, {0.5f, 0.5f, 0.5f}, 1);

	cube.linkChild(&mat);
	secondCube.linkChild(&mat2);
	secondCubeChild.linkChild(&mat2);


	currentTime = previousTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		//clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//update data
		Inputs::checkContinuousInputs();
		currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		//shader section
		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		shader.useThenSetVec3f("uViewPosition", &cameraPos);
		shader.useThenSetMat4f("uView", &view);
		
		for (int i = 0; i < 1000; ++i) {
			cube.draw();
			cube.translate({ -1,0,0 });
		}
		cube.setPosition(cubePosition);
	/*	secondCube.draw();
		secondCubeChild.draw();*/
		//secondCube.draw();

		//light shader section
		LightShader::lightShader().useThenSetMat4f("uView", &view);
		//lightCube.draw();
		pointLight.draw();


		//Swap buffer and check events
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	return 0;
}