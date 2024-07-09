#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.hpp"
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



	//init
	float vertices[] = {//two triangles with common point on the origin
		-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-1.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 1.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		 0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 4,   // first triangle
		2, 3, 4    // second triangle
	};

	//generate VAO(VBO,EBO) which VAO Also stores the vertex attributes
	unsigned int vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);

	unsigned int elementBufferObjectId;
	glGenBuffers(1, &elementBufferObjectId);

	unsigned int verticesBufferObjectId;
	glGenBuffers(1, &verticesBufferObjectId);


	//bind the VAO before the other two
	glBindVertexArray(vertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, verticesBufferObjectId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObjectId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//vertex attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));



	//shaders and uniforms
	Shader shader("shader.vs", "shader.fs");
	shader.useThenSetFloat("offset", 0.1f);
	//main render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		shader.use();
		glBindVertexArray(vertexArrayObject);// no need to bind the vbo and ebo because the vao does it 
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	EXIT_MAIN("");
	return 0;
}