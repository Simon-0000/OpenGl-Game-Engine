#pragma once
#include "GameObject.hpp"
#include <glm/glm.hpp>
#include <format>
struct LightAttenuation {
public:
	LightAttenuation(const float cst, const float lin, const float quad) :constant(cst), linear(lin), quadratic(quad) {}
	LightAttenuation(const float lin, const float quad) :linear(lin),quadratic(quad){}
	const float quadratic,linear,constant = 1;
};

struct LightColors {
	glm::vec3 ambient, diffuse, specular;
};

class DirectionalLight : public Bindable {
public:
	DirectionalLight(Shader* shader, const glm::vec3& direction,
		const LightColors& colors);
	~DirectionalLight();
	void addToShader();
	void localBind() override;
	void localUnbind() override;

private:
	Shader* shader_;
	glm::vec3 direction_;
	LightColors light_;

	short shaderIndex_ = -1;
	static inline unsigned int count_ = 0;
};

class PointLight : public GameObject {
public:
	PointLight(Shader* shader, const Transform& transInfo, const LightAttenuation& attenuation,
		const LightColors& colors);
	~PointLight();
	void addToShader();
	void localBind() override;
	void localUnbind() override;
	void draw() override{}//TODO make a visible light cube 
private:
	LightAttenuation attenuation_;
	LightColors light_;

	short shaderIndex_ = -1;
	static inline unsigned int count_ = 0;


};