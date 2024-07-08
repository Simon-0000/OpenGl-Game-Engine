#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;
#define EXIT_MAIN(message) {cout<<message<<endl; glfwTerminate(); return -1;}
static constexpr int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600, ERROR_MESSAGE_BUFFER_LENGTH = 512;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
unsigned int createAndCompileShader(GLenum shaderType, const char* shaderSrc)
{
	//vertex shader
	unsigned int shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &shaderSrc, NULL);
	glCompileShader(shaderId);

	//check if the shader it compiled correctly:
	int  success;
	char infoLog[ERROR_MESSAGE_BUFFER_LENGTH];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderId, ERROR_MESSAGE_BUFFER_LENGTH, NULL, infoLog);
		std::cout << "ERROR::SHADER::"<<shaderType<<"::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shaderId;
}
unsigned int createShaderProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc) {
	unsigned int vertexShader = createAndCompileShader(GL_VERTEX_SHADER, vertexShaderSrc);
	unsigned int fragmentShader = createAndCompileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
	unsigned int shaderProgramId = glCreateProgram();

	glAttachShader(shaderProgramId, vertexShader);
	glAttachShader(shaderProgramId, fragmentShader);
	glLinkProgram(shaderProgramId);
	glDeleteShader(vertexShader);//dont need the shader once they have been linked 
	glDeleteShader(fragmentShader);
	int success;
	glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
	char infoLog[ERROR_MESSAGE_BUFFER_LENGTH];
	if (!success) {
		glGetProgramInfoLog(shaderProgramId, ERROR_MESSAGE_BUFFER_LENGTH, NULL, infoLog);
		std::cout << "ERROR::SHADER PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	return shaderProgramId;
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
	//other
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	//VBO
	unsigned int verticesBufferObjectId;
	glGenBuffers(1, &verticesBufferObjectId);
	glBindBuffer(GL_ARRAY_BUFFER, verticesBufferObjectId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//VAO //stores which VBO to use + our vertex attribute configuration
	unsigned int vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	//EBO
	unsigned int elementBufferObjectId;
	glGenBuffers(1, &elementBufferObjectId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObjectId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//shaders
	unsigned int shaderProgramId = createShaderProgram(vertexShaderSource, fragmentShaderSource);

	//main render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgramId);
		glBindVertexArray(vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	EXIT_MAIN("");
	return 0;
}