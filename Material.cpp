#include "Material.hpp"

Material::Material(const char* imagePath, Shader* shader, const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spec, float shine) :
	Texture(imagePath,shader,(unsigned int)0,"uMaterial.diffuse"), shader_(shader), ambient(amb), diffuse(diff), specular(spec), shininess(shine)
{
}

void Material::localBind() {
	Texture::localBind();
	shader_->useThenSetVec3f("uMaterial.specular", &specular);
	shader_->useThenSetFloat("uMaterial.shininess", shininess);
}
