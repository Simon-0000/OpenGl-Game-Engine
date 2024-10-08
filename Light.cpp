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
		shader_->setVec3f((name + ".direction").c_str(), &direction_);
		shader_->setVec3f((name + ".light.ambient").c_str(), &light_.ambient);
		shader_->setVec3f((name + ".light.diffuse").c_str(), &light_.diffuse);
		shader_->setVec3f((name + ".light.specular").c_str(), &light_.specular);
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
	:GameObject(shader, transInfo), attenuation_(attenuation), light_(colors)
{
}

PointLight::~PointLight() { --count_;}

void PointLight::addToShader() {
	if (shaderIndex_ == -1) {
		shaderIndex_ = count_++;
		auto name = std::format("uPointLights[{}]", shaderIndex_);
		shader_->useThenSetInt("uPointLightsCount", count_);
		shader_->setVec3f((name + ".light.ambient").c_str(), &light_.ambient);
		shader_->setVec3f((name + ".light.diffuse").c_str(), &light_.diffuse);
		shader_->setVec3f((name + ".light.specular").c_str(), &light_.specular);
		shader_->setFloat((name + ".constant").c_str(), attenuation_.constant);
		shader_->setFloat((name + ".linear").c_str(), attenuation_.linear);
		shader_->setFloat((name + ".quadratic").c_str(), attenuation_.quadratic);
	}

}

void PointLight::localBind() {
	//Model::localBind();
	addToShader();
}

void PointLight::localUnbind()
{
	//Model::localUnbind();

	//TODO implement a system to keep track of all lights in cpu in order to remove them if necessary

}

bool PointLight::tryUpdateModelMatrix()
{
	if (Transform::tryUpdateModelMatrix()) {
		shader_->setVec3f((std::format("uPointLights[{}].position", shaderIndex_)).c_str(), &getGlobalPosition());
		return true;
	}

	return false;
}

SpotLight::SpotLight(Shader* shader, const Transform& transInfo, const LightAttenuation& attenuation, const LightColors& colors, const float angle, const float outerAngle) :
	GameObject(shader, transInfo), attenuation_(attenuation), light_(colors), angle_(angle), outerAngle_(outerAngle)
{

}

void SpotLight::addToShader()
{
	if (shaderIndex_ == -1) {
		shaderIndex_ = count_++;
		auto name = std::format("uSpotLights[{}]", shaderIndex_);
		shader_->useThenSetInt("uSpotLightsCount", count_);
		shader_->setFloat((name + ".angle").c_str(), angle_);
		shader_->setFloat((name + ".outerAngle").c_str(), outerAngle_);

		shader_->setVec3f((name + ".light.ambient").c_str(), &light_.ambient);
		shader_->setVec3f((name + ".light.diffuse").c_str(), &light_.diffuse);
		shader_->setVec3f((name + ".light.specular").c_str(), &light_.specular);
		shader_->setFloat((name + ".constant").c_str(), attenuation_.constant);
		shader_->setFloat((name + ".linear").c_str(), attenuation_.linear);
		shader_->setFloat((name + ".quadratic").c_str(), attenuation_.quadratic);
	}
}

void SpotLight::localBind()
{
	//Model::localBind();
	addToShader();
}

void SpotLight::localUnbind()
{
	//Model::localUnbind();
}

bool SpotLight::tryUpdateModelMatrix()
{
	if (Transform::tryUpdateModelMatrix()) {
		auto name = std::format("uSpotLights[{}]", shaderIndex_);
		shader_->setVec3f((name + ".direction").c_str(), &getForward());
		shader_->setVec3f((name + ".position").c_str(), &getGlobalPosition());
		return true;
	}

	return false;
}
