#include "Shader.hpp"
Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath): id_(createShaderProgram(vertexShaderPath,fragmentShaderPath)) {

}
std::string Shader::readShaderFile(const char* shaderPath) {
	std::ifstream ifs(shaderPath);
	std::stringstream buffer;
	buffer << ifs.rdbuf();
	return buffer.str();
}
void Shader::use() const{
	glUseProgram(id_);
}
void Shader::useThenSetInt(const char* name, int a) const {
	use();
	glUniform1i(getUniformId(name), a);

}

void Shader::useThenSetFloat(const char* name, float a) const {
	use();
	glUniform1f(getUniformId(name), a);

}

void Shader::useThenSetVec2f(const char* name, float a, float b) const {
	use();
	glUniform2f(getUniformId(name), a, b);
}
void Shader::useThenSetVec2f(const char* name, const glm::vec2* vec) const
{
	use();
	glUniform2f(getUniformId(name), vec->x,vec->y);
}


void Shader::useThenSetVec3f(const char* name, float a, float b, float c) const {
	use();
	glUniform3f(getUniformId(name), a, b, c);

}
void Shader::useThenSetVec3f(const char* name, const glm::vec3* vec) const
{
	use();
	glUniform3f(getUniformId(name), vec->x, vec->y,vec->z);
}

void Shader::useThenSetVec4f(const char* name, float a, float b, float c, float d) const {
	use();
	glUniform4f(getUniformId(name), a,b,c,d);
}
void Shader::useThenSetVec4f(const char* name, const glm::vec4* vec) const
{
	use();
	glUniform4f(getUniformId(name), vec->x, vec->y, vec->z, vec->w);

}


void Shader::useThenSetMat4f(const char* name, const glm::mat4* mat) const {
	use();
	glUniformMatrix4fv(getUniformId(name), 1, GL_FALSE, glm::value_ptr(*mat));
}



int Shader::getUniformId(const char* name) const {
	int location = glGetUniformLocation(id_, name);
	if (location == -1)
		throw std::invalid_argument(std::format("The uniform {} doesnt exist",name));
	return location;
}



unsigned int Shader::createAndCompileShader(GLenum shaderType, const char* shaderSrc)
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
		std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shaderId;
}
unsigned int Shader::createShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath) {
	unsigned int vertexShader = createAndCompileShader(GL_VERTEX_SHADER, readShaderFile(vertexShaderPath).c_str());
	unsigned int fragmentShader = createAndCompileShader(GL_FRAGMENT_SHADER, readShaderFile(fragmentShaderPath).c_str());
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