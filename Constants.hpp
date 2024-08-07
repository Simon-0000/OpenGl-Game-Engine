#pragma once

#include "Shader.hpp"

namespace LightShader {
	static Shader& unlitShader() {
		static Shader& shader = Shader::tryCreateShader("unlitShader.vs", "unlitShader.fs");
		return shader;
	}
	static Shader& litShader() {
		static Shader& shader = Shader::tryCreateShader("shader.vs", "shader.fs");
		return shader;
	}
};

