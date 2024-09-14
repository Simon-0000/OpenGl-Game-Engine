#pragma once

//#include "Shader.hpp"
class Shader;

namespace Window {
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 600;

}

namespace ShaderNames {
	static const char* shaderVs = "shader.vs";
	static const char* shaderFs = "shader.fs";

	static const char* unlitShaderVs = "unlitShader.vs";
	static const char* unlitShaderFs = "unlitShader.fs";

	static const char* framebufferShaderVs = "customShader.vs";
	static const char* framebufferShaderFs = "customShader.fs";

	static const char* skyboxShaderVs = "cubemapShader.vs";
	static const char* skyboxShaderFs = "cubemapShader.fs";
};

namespace Debug {
	static constexpr unsigned int ERROR_MESSAGE_BUFFER_LENGTH = 512;
};

