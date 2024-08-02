#include "Material.hpp"

Material::Material(const char* diffuseImagePath, const char* specularImagePath, Shader* shader, float shine) :
	shininess(shine), shader_(shader), 
	diffuseImage_(Texture::tryCreateTexture(diffuseImagePath, shader, (unsigned int)0, "uMaterial.diffuse")),
	specularImage_(Texture::tryCreateTexture(specularImagePath, shader, (unsigned int)1, "uMaterial.specular"))
{
	this->linkChild(&diffuseImage_);
	this->linkChild(&specularImage_);
}

void Material::localBind() {
	diffuseImage_.localBind();
	specularImage_.localBind();
	shader_->useThenSetFloat("uMaterial.shininess", shininess);
}

void Material::localUnbind() {

}