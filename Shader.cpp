#include "Shader.hpp"
Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath): id_(createShaderProgram(vertexShaderPath,fragmentShaderPath)) {

}
Shader& Shader::tryCreateShader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string key = std::string(vertexShaderPath) + fragmentShaderPath;
	if (loadedShaders_.find(key) != loadedShaders_.end())
	{
		return loadedShaders_[key];
	}
	loadedShaders_[key] = Shader(vertexShaderPath,fragmentShaderPath);
	return loadedShaders_[key];
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
void Shader::useThenSetInt(const char* name, int a) const 
{
	use();
	setInt(name, a);
}
void Shader::setInt(const char* name, int a) const
{
	glUniform1i(getUniformId(name), a);
}


void Shader::useThenSetFloat(const char* name, float a) const {
	use();
	setFloat(name, a);
}
void Shader::setFloat(const char* name, float a) const {
	glUniform1f(getUniformId(name), a);
}


void Shader::useThenSetVec2f(const char* name, float a, float b) const 
{
	use();
	setVec2f(name, a, b);
}
void Shader::useThenSetVec2f(const char* name, const glm::vec2* vec) const
{
	use();
	setVec2f(name, vec);
}
void Shader::setVec2f(const char* name, float a, float b) const {
	glUniform2f(getUniformId(name), a, b);
}
void Shader::setVec2f(const char* name, const glm::vec2* vec) const
{
	glUniform2f(getUniformId(name), vec->x, vec->y);
}



void Shader::useThenSetVec3f(const char* name, float a, float b, float c) const 
{
	use();
	setVec3f(name, a,b,c);

}
void Shader::useThenSetVec3f(const char* name, const glm::vec3* vec) const
{
	use();
	setVec3f(name, vec);
}
void Shader::setVec3f(const char* name, float a, float b, float c) const 
{
	glUniform3f(getUniformId(name), a, b, c);
}
void Shader::setVec3f(const char* name, const glm::vec3* vec) const
{
	glUniform3f(getUniformId(name), vec->x, vec->y, vec->z);
}


void Shader::useThenSetVec4f(const char* name, float a, float b, float c, float d) const 
{
	use();
	setVec4f(name, a,b,c,d);
}
void Shader::useThenSetVec4f(const char* name, const glm::vec4* vec) const
{
	use();
	setVec4f(name, vec);
}
void Shader::setVec4f(const char* name, float a, float b, float c, float d) const
{
	glUniform4f(getUniformId(name), a, b, c, d);
}
void Shader::setVec4f(const char* name, const glm::vec4* vec) const
{
	glUniform4f(getUniformId(name), vec->x, vec->y, vec->z, vec->w);
}



void Shader::useThenSetMat4f(const char* name, const glm::mat4* mat) const {
	use();
	setMat4f(name, mat);
}
void Shader::setMat4f(const char* name, const glm::mat4* mat) const {
		glUniformMatrix4fv(getUniformId(name), 1, GL_FALSE, glm::value_ptr(*mat));
}

void Shader::useThenSetMat3f(const char* name, const glm::mat3* mat) const
{
	use();
	setMat3f(name, mat);
}

void Shader::setMat3f(const char* name, const glm::mat3* mat) const
{
	glUniformMatrix3fv(getUniformId(name), 1, GL_FALSE, glm::value_ptr(*mat));

}
template<>
void Shader::setUniform(const char* name, const float* uniformVal) const {setFloat(name, *uniformVal);}
template<>
void Shader::setUniform(const char* name, const int* uniformVal) const {setInt(name, *uniformVal);}
template<>
void Shader::setUniform(const char* name, const glm::vec2* uniformVal) const {setVec2f(name, uniformVal);}
template<>
void Shader::setUniform(const char* name, const glm::vec3* uniformVal) const {setVec3f(name, uniformVal);}
template<>
void Shader::setUniform(const char* name, const glm::mat4* uniformVal) const {setMat4f(name, uniformVal);}






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
	char infoLog[Debug::ERROR_MESSAGE_BUFFER_LENGTH];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderId, Debug::ERROR_MESSAGE_BUFFER_LENGTH, NULL, infoLog);
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
	char infoLog[Debug::ERROR_MESSAGE_BUFFER_LENGTH];
	if (!success) {
		glGetProgramInfoLog(shaderProgramId, Debug::ERROR_MESSAGE_BUFFER_LENGTH, NULL, infoLog);
		std::cout << "ERROR::SHADER PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	return shaderProgramId;
}