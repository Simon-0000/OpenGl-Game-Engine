
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


	//shaders and uniforms
	Shader shader("shader.vs", "shader.fs");
	Shader lightShader("unlitShader.vs", "unlitShader.fs");
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	shader.useThenSetVec3f("uLightColor", 1.0f, 1.0f, 1.0f);
	shader.useThenSetVec3f("uLightPosition", &lightPosition);
	shader.useThenSetMat4f("uProjection", &projection);
	lightShader.useThenSetMat4f("uProjection", &projection);


	//GameObjects setup
	Primitive cube = Primitive::cube(&shader, { cubePosition,{0, glm::pi<float>() / 3,0} });
	Primitive secondCube = Primitive::cube(&shader, { secondCubePosition });
	Primitive secondCubeChild = Primitive::cube(&shader, { secondCubeChildPosition },0.1f);
	secondCubeChild.setParent(&secondCube);

	Primitive light = Primitive::cube(&lightShader, {lightPosition}, 0.1f);


	//textures
	Texture textureA("container.jpg", &shader);
	cube.linkChild(&textureA);
	secondCubeChild.linkChild(&textureA);
	Texture textureB("coolGuy.png", &shader);
	secondCube.linkChild(&textureB);


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
		
		cube.draw();
		secondCube.draw();
		secondCubeChild.draw();
		//secondCube.draw();

		//light shader section
		lightShader.useThenSetMat4f("uView", &view);
		light.draw();



		//Swap buffer and check events
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	return 0;
}