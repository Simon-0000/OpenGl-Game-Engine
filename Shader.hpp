#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <format>
#include <glad/glad.h>
#include "OpenGLDebug.hpp"

class Shader {
public:
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	void use() const;
	void useThenSetVec4f(const char* name, float a, float b, float c, float d) const;
	void useThenSetVec3f(const char* name, float a, float b, float c) const;
private:
	int getUniformId(const char* name) const;
	static std::string readShaderFile(const char* shaderPath);
	static unsigned int createAndCompileShader(GLenum shaderType, const char* shaderSrc);
	static unsigned int createShaderProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc);

	unsigned int id_;

};