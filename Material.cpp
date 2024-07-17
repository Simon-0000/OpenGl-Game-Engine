#include "Material.hpp"

Material::Material(Shader* shader, const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spec, float shine) : 
	shader_(shader), ambient(amb),diffuse(diff),specular(spec),shininess(shine)
{
}

void Material::localBind() {
	shader_->useThenSetVec3f("uMaterial.ambient", &ambient);
	shader_->useThenSetVec3f("uMaterial.diffuse", &diffuse);
	shader_->useThenSetVec3f("uMaterial.specular", &specular);
	shader_->useThenSetFloat("uMaterial.shininess", shininess);
}

void Material::localUnbind() {

}