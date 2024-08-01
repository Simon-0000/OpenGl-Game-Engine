#include "Light.hpp"

DirectionalLight::DirectionalLight(Shader* shader, const glm::vec3& direction, const LightColors& colors):
	shader_(shader), direction_(direction), light_(colors)
{
}

DirectionalLight::~DirectionalLight() { --count_;}

void DirectionalLight::addToShader()
{
	if (shaderIndex_ == -1)
	{
		shaderIndex_ = count_++;
		auto name = std::format("uDirectionalLights[{}]", shaderIndex_);
		shader_->useThenSetInt("uDirectionalLightsCount", count_);
		shader_->useThenSetVec3f((name + ".direction").c_str(), &direction_);
		shader_->useThenSetVec3f((name + ".light.ambient").c_str(), &light_.ambient);
		shader_->useThenSetVec3f((name + ".light.diffuse").c_str(), &light_.diffuse);
		shader_->useThenSetVec3f((name + ".light.specular").c_str(), &light_.specular);
	}
}

void DirectionalLight::localBind()
{
	addToShader();
}
void DirectionalLight::localUnbind() 
{
	//TODO implement a system to keep track of all lights in cpu in order to remove them if necessary

}

PointLight::PointLight(Shader* shader, const Transform& transInfo, const LightAttenuation& attenuation, const LightColors& colors)
	:GameObject(shader, transInfo), Model({ Cube(0.1f) }), attenuation_(attenuation), light_(colors)
{
	this->model = this;
}

PointLight::~PointLight() { --count_;}

void PointLight::addToShader() {
	if (shaderIndex_ == -1) {
		shaderIndex_ = count_++;
		auto name = std::format("uPointLights[{}]", shaderIndex_);
		shader_->useThenSetInt("uPointLightsCount", count_);
		shader_->useThenSetVec3f((name + ".light.ambient").c_str(), &light_.ambient);
		shader_->useThenSetVec3f((name + ".light.diffuse").c_str(), &light_.diffuse);
		shader_->useThenSetVec3f((name + ".light.specular").c_str(), &light_.specular);
		shader_->useThenSetFloat((name + ".constant").c_str(), attenuation_.constant);
		shader_->useThenSetFloat((name + ".linear").c_str(), attenuation_.linear);
		shader_->useThenSetFloat((name + ".quadratic").c_str(), attenuation_.quadratic);
	}

}

void PointLight::localBind() {
	Model::localBind();
	addToShader();
}

void PointLight::localUnbind()
{
	Model::localUnbind();

	//TODO implement a system to keep track of all lights in cpu in order to remove them if necessary

}

bool PointLight::tryUpdateModelMatrix()
{
	if (Transform::tryUpdateModelMatrix()) {
		shader_->useThenSetVec3f((std::format("uPointLights[{}].position", shaderIndex_)).c_str(), &getGlobalPosition());
		return true;
	}

	return false;
}

SpotLight::SpotLight(Shader* shader, const Transform& transInfo, const LightColors& colors, const float angle, const float outerAngle) :
	GameObject(shader, transInfo), Model({ Cube(0.1f) }), light_(colors), angle_(angle), outerAngle_(outerAngle)
{
	this->model = this;

}

void SpotLight::addToShader()
{
	if (shaderIndex_ == -1) {
		shaderIndex_ = count_++;
		auto name = std::format("uSpotLights[{}]", shaderIndex_);
		shader_->useThenSetInt("uSpotLightsCount", count_);
		shader_->useThenSetFloat((name + ".angle").c_str(), angle_);
		shader_->useThenSetFloat((name + ".outerAngle").c_str(), outerAngle_);

		shader_->useThenSetVec3f((name + ".light.ambient").c_str(), &light_.ambient);
		shader_->useThenSetVec3f((name + ".light.diffuse").c_str(), &light_.diffuse);
		shader_->useThenSetVec3f((name + ".light.specular").c_str(), &light_.specular);
	}
}

void SpotLight::localBind()
{
	Model::localBind();
	addToShader();
}

void SpotLight::localUnbind()
{
	Model::localUnbind();
}

bool SpotLight::tryUpdateModelMatrix()
{
	if (Transform::tryUpdateModelMatrix()) {
		auto name = std::format("uSpotLights[{}]", shaderIndex_);
		shader_->useThenSetVec3f((name + ".direction").c_str(), &getForward());
		shader_->useThenSetVec3f((name + ".position").c_str(), &getGlobalPosition());
		return true;
	}

	return false;
}
