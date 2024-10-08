
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
#include "Renderer.hpp"
#include "FrameBuffer.hpp"
#include "RenderBuffer.hpp"
#include "Cubemap.hpp"
#include "Skybox.hpp"

using namespace std;


//time
static float deltaTime;

//camera variables

static constexpr float TURN_SENSITIVITY = 0.1f, TRANSLATION_SENSITIVITY = 2.0f;
static bool shiftFunctionsEnabled = false, controlFunctionsEnabled = false;
static double oldXPos, oldYPos;
static float yaw = -90.0f, pitch = 0;
Camera camera({ {0,0,1} }, 45.0f, static_cast<float>(Window::WINDOW_WIDTH)/ Window::WINDOW_HEIGHT, 100.0f);


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
	GLFWwindow* window = glfwCreateWindow(Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT, "OpenGlGameEngine", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create glfw window" << endl;
		throw runtime_error{"GLFW init error"};

	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGL()) {
		cout << "Failed to load GLAD" << endl;
		throw runtime_error{ "GLAD init error" };
	}

	glViewport(0, 0, Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT);//display rendering on the whole window

	glfwGetCursorPos(window, &oldXPos, &oldYPos);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	Shader& shader = Shader::tryCreateShader(ShaderNames::shaderVs, ShaderNames::shaderFs);
	Shader& unlitShader = Shader::tryCreateShader(ShaderNames::unlitShaderVs, ShaderNames::unlitShaderFs);
	Shader& customShader = Shader::tryCreateShader(ShaderNames::framebufferShaderVs, ShaderNames::framebufferShaderFs);
	Shader& cubemapShader = Shader::tryCreateShader(ShaderNames::skyboxShaderVs, ShaderNames::skyboxShaderFs);

	glm::mat3 defaultKernel = { 0,0,0,0,1,0,0,0,0 };
	customShader.useThenSetMat3f("uKernel", &defaultKernel);
	float uOffset = 1.0f / 300;
	customShader.setFloat("uOffset", uOffset);

	camera.rotate({ 0,glm::pi<float>() / 2 , 0 });
	camera.linkShader(&shader, {"uView","uViewPosition"});
	camera.linkShader(&unlitShader,{"uView"});
	camera.linkShader(&cubemapShader, { "uViewSkybox" });

	auto getView = [](Camera* cam) {return glm::lookAt(cam->getPosition(), cam->getPosition() + cam->getForward(), { 0,1.0f,0 }); };
	camera.linkUniform<glm::mat4>("uView", getView);
	camera.linkUniform<glm::vec3>("uViewPosition", [](Camera* cam) { return cam->getGlobalPosition(); });
	camera.linkUniform<glm::mat4>("uViewSkybox", [&getView](Camera* cam) {
		return glm::mat4(glm::mat3(getView(cam)));
		});
	camera.bind();

	//GameObjects setup
	GameObject cube(&shader, { {0,0,0}});
	GameObject cubeB(&shader, { {0,1,0}});
	GameObject quad(&shader, { {0,1,1}});
	Model cubeModelA({ Cube() });
	Model cubeModelB({ Cube() });

	cube.model = &cubeModelA;
	cubeB.model = &cubeModelB;

	/*GameObject backpack(&shader,{{0,5,0}});
	Model backpackModel("ressources/backpack/backpack.obj");
	backpack.model = &backpackModel;*/

	//lights
	LightColors colors;
	colors.ambient = { 0.2f, 0.2f, 0.2f };
	colors.diffuse = { 0.2f, 0.2f, 0.2f };
	colors.specular = { 0.5f, 0.5f, 0.5f };

	LightColors colorsB;
	colorsB.ambient = { 0.1f, 0.1f, 0.1f };
	colorsB.diffuse = { 0.5f, 0.1f,0.1f };
	colorsB.specular = { 1.0f, 0.0f, 0.0f };

	LightColors colorsC({ 0, 1.0f, 0 }, { 1.0f,1.0f,0 });
	LightAttenuation attenuation{ 0.09f,0.032f };

	DirectionalLight directionalLight(&shader, { -1,-1,0 }, colors);
	directionalLight.addToShader();

	PointLight pointLight(&shader, { lightPosition }, { 0.09f,0.032f }, colorsB);
	pointLight.addToShader();
	Inputs::addKeyCallback({ GLFW_KEY_UP,GLFW_PRESS }, [&]() {
		uOffset += 1 / 600.0f;
		customShader.setFloat("uOffset", uOffset);
	});
	Inputs::addKeyCallback({ GLFW_KEY_DOWN,GLFW_PRESS }, [&]() {
		uOffset -= 1 / 600.0f;
		customShader.setFloat("uOffset", uOffset);
	});

	Inputs::addKeyCallback({ GLFW_KEY_1,GLFW_PRESS }, [&]() {
		glm::mat3 kernel = { 1 / 16.0f,2 / 16.0f,1 / 16.0f,2 / 16.0f,4 / 16.0f,2 / 16.0f,1 / 16.0f,2 / 16.0f,2 / 16.0f };
		customShader.useThenSetMat3f("uKernel", &kernel);
	});
	Inputs::addKeyCallback({ GLFW_KEY_2,GLFW_PRESS }, [&]() {
		glm::mat3 kernel = { 2,2,2,2,-15,2,2,2,2 };
		customShader.useThenSetMat3f("uKernel", &kernel);
	});
	Inputs::addKeyCallback({ GLFW_KEY_3,GLFW_PRESS }, [&]() {
		glm::mat3 kernel = { 0,0,0,0,1,0,0,0,0 };
		customShader.useThenSetMat3f("uKernel", &kernel);
	});


	//mats and textures
	Material mat("earth2.jpg", "earth2.jpg", &shader, 100.0f);
	Material matB("coolGuy.png", "coolGuy.png", &shader, 100.0f);

	cubeModelA.meshes[0].linkChild(&mat);
	cubeModelB.meshes[0].linkChild(&matB);

	pointLight.linkChild(&mat);

	//Cubemaps
	float skyboxVertices[] = {
		// positions          
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
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	std::string faces[6]
	{
			"ressources/skybox/right.jpg",
			"ressources/skybox/left.jpg",
			"ressources/skybox/top.jpg",
			"ressources/skybox/bottom.jpg",
			"ressources/skybox/front.jpg",
			"ressources/skybox/back.jpg"
	};
	Cubemap cubemapTexture(faces, &cubemapShader);

	
	//Mesh skybox = Cube(0.5f);
	Skybox skybox(&cubemapTexture);
	//skybox.linkChild(&cubemapTexture);



	//framebuffer and renderBuffer
	
	FrameBuffer fBuffer;
	RenderBuffer rBuffer(fBuffer);

	//Renderer
	Renderer renderer(&camera);
	renderer.addToOpaqueBuffer(&cube);
	renderer.addToOpaqueBuffer(&pointLight);
	renderer.addToTransparentBuffer(&cubeB);




	currentTime = previousTime = glfwGetTime();
	int frameCount = 0;
	float timeElapsed = 0;

	while (!glfwWindowShouldClose(window))
	{
		//clear color and depth buffers 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//update data
		Inputs::checkContinuousInputs();
		currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;
		if (camera.update()) {
			renderer.sortTransparentBuffer();//could optimize by only sorting every few frames (downside is that it will cause small visible glitches)
		}
		if ((timeElapsed += deltaTime) > 1.0f) {
			system("cls");
			std::cout << "FPS:" << frameCount << std::endl;
			timeElapsed = 0;
			frameCount = 0;
		}
		++frameCount;

		
		//RENDERING
		skybox.draw();
		
		renderer.renderBuffers();
		// ... draw rest of the scene


		//fBuffer.renderFrameBuffer(renderer, customShader);


		//Swap buffer and check events
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	return 0;
}