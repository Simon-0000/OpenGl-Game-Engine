#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* imagePath, Shader* shader, unsigned int textureUnit) : textureUnit_(textureUnit){
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

	unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
	if (!data){}
		//throw 

	std::string path(imagePath);
	if(path.ends_with(".png"))
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);//Rgba
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//Rgb
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	shader->useThenSetInt(std::format("{}{}",DEFAULT_SHADER_TEXTURE_NAME, textureUnit_).c_str(), textureUnit_);
}

Texture::Texture(const char* imagePath, Shader * shader, Texture * parentTexture) :Texture(imagePath, shader, parentTexture->textureUnit_ + 1) 
{
	parentTexture->linkChild(this);
}
void Texture::localBind() {
	glActiveTexture(GL_TEXTURE0 + textureUnit_);
	glBindTexture(GL_TEXTURE_2D, id_);
}

int Texture::getNumberOfTextureUnits() {
	int texture_units = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
	return texture_units;
}
