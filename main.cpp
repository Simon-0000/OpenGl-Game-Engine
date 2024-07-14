
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

#define EXIT_MAIN(message) {cout<<message<<endl; glfwTerminate(); return -1;}
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
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

	if (action == GLFW_PRESS)
	{
		switch(key) {
			case GLFW_KEY_ESCAPE:
				break;
			case GLFW_KEY_LEFT_SHIFT:
				shiftFunctionsEnabled = true;
				break;
			case GLFW_KEY_LEFT_CONTROL:
				controlFunctionsEnabled = true;
				break;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_LEFT_SHIFT:
				shiftFunctionsEnabled = false;
				break;
			case GLFW_KEY_LEFT_CONTROL:
				controlFunctionsEnabled = false;
				break;
		}
	}
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


int main() {

	//setup
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGlGameEngine", NULL, NULL);
	if (window == NULL)
		EXIT_MAIN("Failed to create glfw window");

	glfwMakeContextCurrent(window);
	if (!gladLoadGL())
		EXIT_MAIN("Failed to initialize GLAD");

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);//display rendering on the whole window

	glfwGetCursorPos(window, &oldXPos, &oldYPos);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	Inputs::init(window);
	Inputs::addKeyCallback({ GLFW_KEY_ESCAPE ,GLFW_PRESS }, [window]() {glfwSetWindowShouldClose(window, true); });
	Inputs::addKeyCallback({ GLFW_KEY_LEFT_SHIFT ,GLFW_PRESS }, []() {shiftFunctionsEnabled = true; });
	Inputs::addKeyCallback({ GLFW_KEY_LEFT_SHIFT ,GLFW_RELEASE }, []() {shiftFunctionsEnabled = false; });
	Inputs::addKeyCallback({ GLFW_KEY_LEFT_CONTROL ,GLFW_PRESS }, []() {controlFunctionsEnabled = true;  });
	Inputs::addKeyCallback({ GLFW_KEY_LEFT_CONTROL ,GLFW_RELEASE }, []() {controlFunctionsEnabled = false;  });

	Inputs::addContinuousKeyCallback({ GLFW_KEY_W ,GLFW_PRESS }, []() {cameraPos += deltaTime * TRANSLATION_SENSITIVITY * cameraFront; });
	Inputs::addContinuousKeyCallback({ GLFW_KEY_A ,GLFW_PRESS }, []() {cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * TRANSLATION_SENSITIVITY * deltaTime;});
	Inputs::addContinuousKeyCallback({ GLFW_KEY_S ,GLFW_PRESS }, []() {cameraPos -= deltaTime * TRANSLATION_SENSITIVITY * cameraFront; });
	Inputs::addContinuousKeyCallback({ GLFW_KEY_D ,GLFW_PRESS }, []() {cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * TRANSLATION_SENSITIVITY * deltaTime; });



	glEnable(GL_DEPTH_TEST);

	//shaders and uniforms
	Shader shader("shader.vs", "shader.fs");
	Shader lightShader("unlitShader.vs", "unlitShader.fs");


	Primitive cube = Primitive::cube();
	Primitive light = Primitive::cube(0.1f);

	glm::vec3 lightPosition = { -2.0f, 0.0f, 1.65f };
	glm::vec3 cubePosition = { 0.0f, 0.0f, 0.0f };
	shader.useThenSetVec3f("uLightColor", 1.0f, 1.0f, 1.0f);
	shader.useThenSetVec3f("uLightPosition", &lightPosition);

	Texture texture1("container.jpg", &shader);
	//Texture texture2("container.jpg", &shader, 2); //set the other image and link it to texture1 to be able to bind them at the same time
	cube.linkChild(&texture1);
	//cube.linkChild(&texture2);
	//texture1.unbind();
	//texture2.unbind();


	//main render loop
	float currentTime, previousTime;
	currentTime = previousTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		Inputs::checkContinuousInputs();

		currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//CUBE
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePosition);
		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		shader.useThenSetMat4f("uView", &view);
		shader.useThenSetMat4f("uProjection", &projection);
		shader.useThenSetMat4f("uModel", &model);
		
		cube.bind();
		glDrawElements(GL_TRIANGLES, cube.getNumberOfIndices(), GL_UNSIGNED_INT, 0);
		cube.unbind();

		//LIGHT
		auto lightModel = glm::translate(glm::mat4(1.0f), lightPosition);
		lightShader.useThenSetMat4f("uView", &view);
		lightShader.useThenSetMat4f("uProjection", &projection);
		lightShader.useThenSetMat4f("uModel", &lightModel);

		light.bind();
		glDrawElements(GL_TRIANGLES, light.getNumberOfIndices(), GL_UNSIGNED_INT, 0);
		light.unbind();

		shader.useThenSetVec3f("uViewPosition", &cameraPos);


		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	EXIT_MAIN("");
	return 0;
}