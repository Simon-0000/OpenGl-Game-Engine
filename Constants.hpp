#pragma once
#include "Shader.hpp"


namespace LightShader {
	static Shader& lightShader() {
		static Shader shader("unlitShader.vs", "unlitShader.fs");
		return shader;
	}
	enum LightType {
		DIRECTIONAL = 0,
		POSITIONAL = 1
	};
};

