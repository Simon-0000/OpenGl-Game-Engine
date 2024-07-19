#include "Light.hpp"

Light::Light(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
{
}

DirectionalLight::DirectionalLight(const glm::vec3& direction,
	const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular):
	Light(ambient,diffuse,specular)
{
}

PointLight::PointLight(Shader* shader, const Transform& transInfo, 
	const glm::vec3& position, const LightAttenuation& attenuation,
	const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular):
	Light(ambient,diffuse,specular), GameObject(shader,transInfo)
{
}
