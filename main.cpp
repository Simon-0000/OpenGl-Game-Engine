
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.hpp"
#include <vector>
#include <cmath>
#include "Inputs.hpp"
#include "Material.hpp"
#include "Constants.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "GameObject.hpp"



using namespace std;

static constexpr int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

//time
static float deltaTime;

//camera variables

static constexpr float TURN_SENSITIVITY = 0.1f, TRANSLATION_SENSITIVITY = 2.0f;
static bool shiftFunctionsEnabled = false, controlFunctionsEnabled = false;
static double oldXPos, oldYPos;
static float yaw = -90.0f, pitch = 0;
Camera camera({ {0,0,1} }, 60.0f, 800.0f / 600.0f, 100.0f);


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

static void mouse_callback(GLFWwindow* window, double xpos,double ypos){
	if (!controlFunctionsEnabled) {
		
		yaw += (xpos - oldXPos) * TURN_SENSITIVITY;
		pitch = max(min( pitch + ((oldYPos - ypos) * TURN_SENSITIVITY), 89.9), -89.9);
		camera.setRotation( glm::quat(-glm::vec3(glm::radians(pitch), glm::radians(yaw),0)));
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

	Inputs::addContinuousKeyCallback({ GLFW_KEY_W ,GLFW_PRESS }, []() {camera.translate(deltaTime * TRANSLATION_SENSITIVITY * camera.getForward()); });
	Inputs::addContinuousKeyCallback({ GLFW_KEY_A ,GLFW_PRESS }, []() {camera.translate(camera.getRight() * TRANSLATION_SENSITIVITY * deltaTime); });
	Inputs::addContinuousKeyCallback({ GLFW_KEY_S ,GLFW_PRESS }, []() {camera.translate(-deltaTime * TRANSLATION_SENSITIVITY * camera.getForward()); });
	Inputs::addContinuousKeyCallback({ GLFW_KEY_D ,GLFW_PRESS }, []() {camera.translate(camera.getRight() * -TRANSLATION_SENSITIVITY * deltaTime); });

	Inputs::addContinuousKeyCallback({ GLFW_KEY_E ,GLFW_PRESS }, []() {camera.translate(deltaTime * TRANSLATION_SENSITIVITY * camera.getUp()); });
	Inputs::addContinuousKeyCallback({ GLFW_KEY_Q ,GLFW_PRESS }, []() {camera.translate(-deltaTime * TRANSLATION_SENSITIVITY * camera.getUp()); });


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
	glm::vec3 lightPosition = { -5.0f, 0.0f, 1.65f };
	glm::vec3 cubePosition = { 0.0f, 0.0f, 0.0f };
	float currentTime, previousTime;


	//shaders
	Shader& shader = LightShader::litShader();


	camera.linkShader(&shader);
	camera.linkShader(&LightShader::unlitShader());
	camera.bind();

	//GameObjects setup
	GameObject cube(&shader, { cubePosition,{0, glm::pi<float>() / 3,0}});
	GameObject cubeB(&LightShader::unlitShader(), { cubePosition,{0, glm::pi<float>() / 3,0} });

	Model cubeModelA({ Cube() });
	Model cubeModelB({ Cube() });

	cube.model = &cubeModelA;
	cubeB.model = &cubeModelB;

	//GameObject backpack(&shader,{{0,10,0}});
	//Model backpackModel("ressources/backpack/backpack.obj");
	//backpack.model = &backpackModel;

	//lights
	LightColors colors;
	colors.ambient = { 0.05f, 0.05f, 0.05f };
	colors.diffuse = { 0.2f, 0.2f, 0.2f };
	colors.specular = { 0.5f, 0.5f, 0.5f };

	LightColors colorsB;
	colorsB.ambient = { 0.02f, 0.02f, 0.02f };
	colorsB.diffuse = { 0.5f,0.1f,0.1f };
	colorsB.specular = { 1.0f, 0.0f, 0.0f };

	LightColors colorsC({ 0, 1.0f, 0 }, { 1.0f,1.0f,0 });
	LightAttenuation attenuation{ 0.09f,0.032f };

	DirectionalLight directionalLight(&shader, { -1,-1,0 }, colors);
	directionalLight.addToShader();
	PointLight pointLight(&shader, { lightPosition }, { 0.09f,0.032f }, colorsB);
	PointLight pointLight2(&shader, { {0,1,0} }, attenuation, colorsB);
	SpotLight pointLight3(&shader, { {0,-2,0} }, attenuation, colorsC, 0.91f,0.88f);
	Inputs::addContinuousKeyCallback({ GLFW_KEY_RIGHT ,GLFW_PRESS }, [&]() { pointLight.rotate({deltaTime * glm::pi<float>() / 4,0,0 }); });
	Inputs::addContinuousKeyCallback({ GLFW_KEY_LEFT ,GLFW_PRESS }, [&]() { pointLight.rotate({-deltaTime * glm::pi<float>() / 4,0,0 }); });
	Inputs::addContinuousKeyCallback({ GLFW_KEY_UP ,GLFW_PRESS }, [&]() { pointLight.translate({ deltaTime * -2,0,0 }); });
	Inputs::addContinuousKeyCallback({ GLFW_KEY_DOWN ,GLFW_PRESS }, [&]() { pointLight.translate({ deltaTime * 2,0,0 }); });
	
	pointLight2.setParent(&pointLight);
	pointLight3.setParent(&pointLight2);
	pointLight.addToShader();
	pointLight2.addToShader();
	pointLight3.addToShader();



	Material mat("container2.png", "container2_specular.png", &shader, 100.0f);
	Material matB("container.jpg", "container2_specular.png", &shader, 100.0f);

	cubeModelA.meshes[0].linkChild(&mat);
	cubeModelB.meshes[0].linkChild(&matB);

	pointLight.linkChild(&mat);
	pointLight2.linkChild(&mat);
	pointLight3.linkChild(&mat);


	currentTime = previousTime = glfwGetTime();
	int frameCount = 0;
	float timeElapsed = 0;

	Inputs::addKeyCallback({ GLFW_KEY_P ,GLFW_PRESS }, [&]() {
		if (cube.shader_ == &shader)
		{
			cube.shader_ = &LightShader::unlitShader();
			cubeB.shader_ = &shader;
		}
		else {
			cubeB.shader_ = &LightShader::unlitShader();
			cube.shader_ = &shader;
		}
		}
	);

	while (!glfwWindowShouldClose(window))
	{
		//clear color and depth buffers 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//update data
		Inputs::checkContinuousInputs();
		currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;
		camera.update();
		if ((timeElapsed += deltaTime) > 1.0f) {
			system("cls");
			std::cout << "FPS:" << frameCount << std::endl;
			timeElapsed = 0;
			frameCount = 0;
		}
		++frameCount;
		//shader section

		//GameObject::drawWithOutline({ &cube }, {0.0f,1.0f,0.0f});
		//auto temp = cube.shader_;
		//cube.shader_ = &LightShader::unlitShader();
		//LightShader::unlitShader().useThenSetVec3f("uLightColor", 0.0f,0.0f,1.0f);
		//cube.shader_ = &LightShader::unlitShader();
		//cube.scale(glm::vec3(0.1f));
		//cube.localDraw();
		//cube.scale(glm::vec3(-0.1f));
		//cube.shader_ = &LightShader::litShader();
		//GameObject::drawWithOutline({ &cube }, { 0,0,1.0f });

		GameObject::drawWithOutline({ &cube }, { 0,1.0f,1.0f });
		auto a = LightShader::unlitShader();

		//LightShader::unlitShader().useThenSetVec3f("uLightColor", 0, 1, 0);
		cube.setPosition({ 0,0,0 });
		//cube.draw();
		cubeB.setPosition({ 0,1,0 });
		//cubeB.draw();
	/*	LightShader::unlitShader().useThenSetVec3f("uLightColor", 0, 1, 0);

		cube.shader_ = &shader;
		cube.setPosition(cubePosition);
		cube.draw();
		cube.setPosition(cubePosition + glm::vec3{ 0, 1.5f, 0 });
		cube.shader_ = &LightShader::unlitShader();
		cube.localDraw();*/

		//cube.shader_ = temp;

		//for (int i = 0; i < 500; ++i) {
		//	cube.translate({ -1,0,0 });
		//	cube.draw();
		//}
		pointLight.draw();
		//pointLight2.draw();
		//pointLight3.draw();
		//backpack.draw();


		//Swap buffer and check events
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	return 0;
}