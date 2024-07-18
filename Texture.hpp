#pragma once
#include <iostream>
#include <string>
#include <format>
#include "Shader.hpp"
#include "Bindable.hpp"


class Texture : public Bindable{
public:
	Texture(const char* imagePath, Shader* shader, unsigned int textureUnit = 0, const char* textureName = DEFAULT_SHADER_TEXTURE_NAME);

	void localBind() override;
	void localUnbind() override;

private:
	int getNumberOfTextureUnits();

	unsigned int id_, textureUnit_;
	static inline const char* DEFAULT_SHADER_TEXTURE_NAME = "texture";
};