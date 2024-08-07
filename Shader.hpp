#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <format>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGLDebug.hpp"

class Shader {
public:
	Shader() = default;
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	static Shader& tryCreateShader(const char* vertexShaderPath, const char* fragmentShaderPath);

	void use() const;

	void useThenSetInt(const char* name, int a) const;
	void setInt(const char* name, int a) const;

	void useThenSetFloat(const char* name, float a) const;
	void setFloat(const char* name, float a) const;

	void useThenSetVec2f(const char* name, float a, float b) const;
	void useThenSetVec2f(const char* name, const glm::vec2* vec) const;
	void setVec2f(const char* name, float a, float b) const;
	void setVec2f(const char* name, const glm::vec2* vec) const;

	void useThenSetVec3f(const char* name, float a, float b, float c) const;
	void useThenSetVec3f(const char* name, const glm::vec3* vec) const;
	void setVec3f(const char* name, float a, float b, float c) const;
	void setVec3f(const char* name, const glm::vec3* vec) const;

	void useThenSetVec4f(const char* name, float a, float b, float c, float d) const;
	void useThenSetVec4f(const char* name, const glm::vec4* vec) const;
	void setVec4f(const char* name, float a, float b, float c, float d) const;
	void setVec4f(const char* name, const glm::vec4* vec) const;


	void useThenSetMat4f(const char* name, const glm::mat4* mat) const;
	void setMat4f(const char* name, const glm::mat4* mat) const;


private:
	int getUniformId(const char* name) const;
	static std::string readShaderFile(const char* shaderPath);
	static unsigned int createAndCompileShader(GLenum shaderType, const char* shaderSrc);
	static unsigned int createShaderProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc);

	unsigned int id_;
	static inline std::unordered_map<std::string, Shader> loadedShaders_ = {};
};