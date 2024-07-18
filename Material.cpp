#include "Material.hpp"

Material::Material(const char* diffuseImagePath, const char* specularImagePath, Shader* shader, const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spec, float shine) :
	ambient(amb), diffuse(diff), specular(spec), shininess(shine), shader_(shader), 
	diffuseImage_(diffuseImagePath, shader, (unsigned int)0, "uMaterial.diffuse"),
	specularImage_(specularImagePath, shader, (unsigned int)1, "uMaterial.specular")
{
	this->linkChild(&diffuseImage_);
	this->linkChild(&specularImage_);
}

void Material::localBind() {
	shader_->useThenSetVec3f("uMaterial.specular", &specular);
	shader_->useThenSetFloat("uMaterial.shininess", shininess);
}

void Material::localUnbind() {

}