#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* imagePath, Shader* shader, unsigned int textureUnit, const char* textureName) : textureUnit_(textureUnit){
	static int MAX_TEXTURE_UNITS = getNumberOfTextureUnits();
	if (textureUnit_ >= MAX_TEXTURE_UNITS)
		throw std::range_error(std::format("Texture unit location out of maximum range: {}",MAX_TEXTURE_UNITS));

	int width, height, nrChannels;
	glGenTextures(1, &id_);
	glActiveTexture(GL_TEXTURE0 + textureUnit_);
	glBindTexture(GL_TEXTURE_2D, id_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
	if (!data){}
		//throw 

	std::string path(imagePath);
	if(path.ends_with(".png"))
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);//Rgba
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//Rgb
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	shader->useThenSetInt(textureName, textureUnit_);
}

void Texture::localBind() {
	glActiveTexture(GL_TEXTURE0 + textureUnit_);
	glBindTexture(GL_TEXTURE_2D, id_);
}
void Texture::localUnbind() {
	glActiveTexture(GL_TEXTURE0 + textureUnit_);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::getNumberOfTextureUnits() {
	int texture_units = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
	return texture_units;
}
