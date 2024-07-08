#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;
#define EXIT_MAIN(message) {cout<<message<<endl; glfwTerminate(); return -1;}
static constexpr int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main() {
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

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	EXIT_MAIN("");
	return 0;
}