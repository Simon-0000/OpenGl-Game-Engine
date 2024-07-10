
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

using namespace std;

#define EXIT_MAIN(message) {cout<<message<<endl; glfwTerminate(); return -1;}
static constexpr int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	glEnable(GL_DEPTH_TEST);

	//shaders and uniforms
	Shader shader("shader.vs", "shader.fs");

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
	};

	Primitive cube = Primitive::cube();
	Texture texture1("coolGuy.png", &shader,1);
	Texture texture2("container.jpg", &shader, 2); //set the other image and link it to texture1 to be able to bind them at the same time
	cube.linkChild(&texture1);
	cube.linkChild(&texture2);
	texture1.unbind();
	texture2.unbind();

	//main render loop
	double previousTime = glfwGetTime();
	int fps = 0;
	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		++fps;
		if (currentTime - previousTime >= 5.0)
		{
			system("cls");
			std::cout << "FPS: " << fps/5<<std::endl;
			fps = 0;
			previousTime = currentTime;
		}
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		shader.useThenSetMat4f("view", &view);
		shader.useThenSetMat4f("projection", &projection);


		cube.bind();
		for (unsigned int i = 0; i < 4; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (float)currentTime * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.useThenSetMat4f("model", &model);

			glDrawElements(GL_TRIANGLES, cube.getNumberOfIndices(), GL_UNSIGNED_INT, 0);
		}
		cube.unbind();


		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	EXIT_MAIN("");
	return 0;
}