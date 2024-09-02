#pragma once
#include <iostream>
#include <string>
#include "Bindable.hpp"
#include "Shader.hpp"

class Cubemap: public Bindable {
public:
	Cubemap(std::string locations[6], Shader* shader, unsigned int textureUnit = 0, const char* textureName = DEFAULT_SHADER_CUBEMAP_NAME);
	void localBind() override;
	void localUnbind() override;
private:
	unsigned int id_, textureUnit_;
	static inline const char* DEFAULT_SHADER_CUBEMAP_NAME = "uCubemap";

};