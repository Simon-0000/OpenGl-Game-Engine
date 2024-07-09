
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.hpp"
#include "Texture.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left 
	};
	unsigned int indices[] = {
		0, 1, 2,   // first triangle
		2, 3, 0    // second triangle
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	//shaders and uniforms
	Shader shader("shader.vs", "shader.fs");

	Texture texture1("coolGuy.png", &shader,1);
	Texture texture2("container.jpg", &shader, &texture1); //set the other image and link it to texture1 to be able to bind them at the same time



	//main render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT);
		//glm
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		shader.useThenSetMat4f("transform", &trans);

		texture1.bind();
		glBindVertexArray(vertexArrayObject);// no need to bind the vbo and ebo because the vao does it 
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	EXIT_MAIN("");
	return 0;
}