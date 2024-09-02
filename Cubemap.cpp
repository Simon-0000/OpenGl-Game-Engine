#include "Cubemap.hpp"
//assumes that #define STB_IMAGE_IMPLEMENTATION is defined in main
#include "stb_image.h"


Cubemap::Cubemap(std::string locations[6], Shader* shader, unsigned int textureUnit, const char* textureName) :
    textureUnit_(textureUnit)
{
	glGenTextures(1,&id_);
    glActiveTexture(GL_TEXTURE0 + textureUnit_);
    localBind();
    int width, height, nrChannels;
    unsigned char* data;
    int a = sizeof(locations) / sizeof(std::string);
    for (unsigned int i = 0; i < sizeof(locations)/sizeof(std::string); ++i)
    {
        data = stbi_load(locations[i].c_str(), &width, &height, &nrChannels, 0);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
        );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    shader->setInt(textureName, textureUnit_);
}

void Cubemap::localBind()
{
    glActiveTexture(GL_TEXTURE0 + textureUnit_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id_);
}
void Cubemap::localUnbind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}