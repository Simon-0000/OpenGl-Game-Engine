#pragma once
#include "Shader.hpp"


namespace LightShader {
	static Shader& unlitShader() {
		static Shader shader("unlitShader.vs", "unlitShader.fs");
		return shader;
	}
	static Shader& litShader() {
		static Shader shader("shader.vs", "shader.fs");
		return shader;
	}
};

