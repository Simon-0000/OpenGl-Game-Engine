#pragma once
#include "GameObject.hpp"
#include <glm/glm.hpp>

struct LightAttenuation {
public:
	LightAttenuation(const float constant, const float linear, const float quadratic) :constant_(constant), linear_(linear), quadratic_(quadratic) {}
	LightAttenuation(const float linear, const float quadratic) :linear_(linear),quadratic_(quadratic){}
	const float quadratic_,linear_,constant_ = 1;
};

class Light {
public:
	Light(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

protected:
	unsigned int id_;
	//const glm::vec3 ambient, diffuse, specular;
};

class DirectionalLight : public Light {
public:
	DirectionalLight(const glm::vec3& direction,
		const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
private:
	static inline unsigned int count_ = 0;
};

class PointLight : public Light, public GameObject {
public:
	PointLight(Shader* shader, const Transform& transInfo,
		const glm::vec3& position, const LightAttenuation& attenuation,
		const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
private:
	static inline unsigned int count_ = 0;


};