#pragma once

#include "Shader.hpp"

namespace Window {
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 600;

}

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

