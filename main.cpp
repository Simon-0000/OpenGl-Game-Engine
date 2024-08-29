
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

using namespace std;


//time
static float deltaTime;

//camera variables

static constexpr float TURN_SENSITIVITY = 0.1f, TRANSLATION_SENSITIVITY = 2.0f;
static bool shiftFunctionsEnabled = false, controlFunctionsEnabled = false;
static double oldXPos, oldYPos;
static float yaw = -90.0f, pitch = 0;
Camera camera({ {0,0,1} }, 60.0f, static_cast<float>(Window::WINDOW_WIDTH)/ Window::WINDOW_HEIGHT, 100.0f);


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
	Shader& shader = LightShader::litShader();
	Shader& customShader = Shader::tryCreateShader("customShader.vs", "customShader.fs");
	
	camera.rotate({ 0,glm::pi<float>() / 2 , 0 });
	camera.linkShader(&shader);
	camera.linkShader(&LightShader::unlitShader());
	camera.bind();

	//GameObjects setup
	GameObject cube(&shader, { {0,0,0}});
	GameObject cubeB(&shader, { {0,1,0}});
	GameObject quad(&shader, { {0,1,1}});
	Model cubeModelA({ Cube() });
	Model cubeModelB({ Cube() });
	Model quadModel({ Quad() });

	cube.model = &cubeModelA;
	cubeB.model = &cubeModelB;

	//GameObject backpack(&shader,{{0,5,0}});
	//Model backpackModel("ressources/backpack/backpack.obj");
	//backpack.model = &backpackModel;

	//lights
	LightColors colors;
	colors.ambient = { 0.6f, 0.6f, 0.6f };
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

	Inputs::addContinuousKeyCallback({ GLFW_KEY_RIGHT ,GLFW_PRESS }, [&]() { pointLight.rotate({deltaTime * glm::pi<float>() / 4,0,0 }); });
	Inputs::addContinuousKeyCallback({ GLFW_KEY_LEFT ,GLFW_PRESS }, [&]() { pointLight.rotate({-deltaTime * glm::pi<float>() / 4,0,0 }); });
	Inputs::addContinuousKeyCallback({ GLFW_KEY_UP ,GLFW_PRESS }, [&]() { pointLight.translate({ deltaTime * -2,0,0 }); });
	Inputs::addContinuousKeyCallback({ GLFW_KEY_DOWN ,GLFW_PRESS }, [&]() { pointLight.translate({ deltaTime * 2,0,0 }); });
	



	//mats and textures
	Material mat("earth2.jpg", "earth2.jpg", &shader, 100.0f);
	Material matB("coolGuy.png", "coolGuy.png", &shader, 100.0f);

	cubeModelA.meshes[0].linkChild(&mat);
	cubeModelB.meshes[0].linkChild(&matB);

	pointLight.linkChild(&mat);


	//framebuffer and renderBuffer
	
	//FrameBuffer fBuffer(&customShader);
	//RenderBuffer rBuffer(fBuffer);
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	//Renderer
	Renderer renderer(&camera);
	renderer.addToOpaqueBuffer(&cube);
	renderer.addToOpaqueBuffer(&pointLight);
	renderer.addToTransparentBuffer(&cubeB);
	//renderer.addToTransparentBuffer(&quad);


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
		// first pass
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
		glEnable(GL_DEPTH_TEST);
		//DrawScene();

		cube.draw();
		// second pass
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		customShader.use();
		glBindVertexArray(quadVAO);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);




		//Swap buffer and check events
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	return 0;
}