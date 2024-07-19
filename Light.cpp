#include "Light.hpp"

DirectionalLight::DirectionalLight(Shader* shader, const glm::vec3& direction, const LightColors& colors):
	shader_(shader), direction_(direction), light_(colors)
{
}

DirectionalLight::~DirectionalLight() { --count_;}

void DirectionalLight::addToShader()
{
	shaderIndex_ = count_++;
	auto name = std::format("uDirectionalLights[{}]",shaderIndex_);
	shader_->useThenSetInt("uDirectionalLightsCount", count_);
	shader_->useThenSetVec3f((name + ".direction").c_str(), &direction_);
	shader_->useThenSetVec3f((name + ".light.ambient").c_str(), &light_.ambient);
	shader_->useThenSetVec3f((name + ".light.diffuse").c_str(), &light_.diffuse);
	shader_->useThenSetVec3f((name + ".light.specular").c_str(), &light_.specular);
}

void DirectionalLight::localBind()
{
	if (shaderIndex_ == -1)
		addToShader();
}
void DirectionalLight::localUnbind() 
{
	//TODO implement a system to keep track of all lights in cpu in order to remove them if necessary

}

PointLight::PointLight(Shader* shader, const Transform& transInfo, const LightAttenuation& attenuation, const LightColors& colors)
:Cube(shader, transInfo, 0.1f), attenuation_(attenuation), light_(colors)
{
}

PointLight::~PointLight() { --count_;}

void PointLight::addToShader() {
	shaderIndex_ = count_++;
	auto name = std::format("uPointLights[{}]", shaderIndex_);
	shader_->useThenSetInt("uPointLightsCount", count_);
	shader_->useThenSetVec3f((name + ".position").c_str(), &getPosition());
	shader_->useThenSetVec3f((name + ".light.ambient").c_str(), &light_.ambient);
	shader_->useThenSetVec3f((name + ".light.diffuse").c_str(), &light_.diffuse);
	shader_->useThenSetVec3f((name + ".light.specular").c_str(), &light_.specular);
	shader_->useThenSetFloat((name + ".constant").c_str(), attenuation_.constant);
	shader_->useThenSetFloat((name + ".linear").c_str(), attenuation_.linear);
	shader_->useThenSetFloat((name + ".quadratic").c_str(), attenuation_.quadratic);

}

void PointLight::localBind() {
	Primitive::localBind();
	if (shaderIndex_ == -1)
		addToShader();
}

void PointLight::localUnbind()
{
	Primitive::localUnbind();

	//TODO implement a system to keep track of all lights in cpu in order to remove them if necessary

}

bool PointLight::tryUpdateModelMatrix()
{
	if (Transform::tryUpdateModelMatrix()) {

		glm::vec3 position = getModelMatrix() * glm::vec4(getPosition(),1.0f);
		shader_->useThenSetVec3f((std::format("uPointLights[{}].position", shaderIndex_)).c_str(), &position);
		return true;
	}

	return false;
}

//void PointLight::updateModelMatrix()
//{

//}
