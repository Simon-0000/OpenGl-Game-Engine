#pragma once
#include <iostream>
#include <string>
#include <format>
#include <unordered_map>
#include "Shader.hpp"
#include "Bindable.hpp"
#include "Constants.hpp"

class Texture : public Bindable{
public:
	//Texture() = default;
	Texture();
	Texture(const char* imagePath, Shader* shader, unsigned int textureUnit = 0, const char* textureName = DEFAULT_SHADER_TEXTURE_NAME);

	static Texture& tryCreateTexture(const char* imagePath, Shader* shader, unsigned int textureUnit = 0, const char* textureName = DEFAULT_SHADER_TEXTURE_NAME);
	unsigned int getId();

	void localBind() override;
	void localUnbind() override;

private:
	int getNumberOfTextureUnits();

	unsigned int id_, textureUnit_;
	static inline const char* DEFAULT_SHADER_TEXTURE_NAME = "texture";
	static inline std::unordered_map<std::string, Texture> loadedTextures_ = {};
};