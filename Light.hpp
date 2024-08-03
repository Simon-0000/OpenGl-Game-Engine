#pragma once
#include "Mesh.hpp"
#include "GameObject.hpp"
#include "Constants.hpp"
#include <glm/glm.hpp>
#include <format>

struct LightAttenuation {
	LightAttenuation(const float cst, const float lin, const float quad) :constant(cst), linear(lin), quadratic(quad) {}
	LightAttenuation(const float lin, const float quad) :linear(lin),quadratic(quad){}
	const float quadratic,linear,constant = 1;
};

struct LightColors {
	LightColors() = default;
	LightColors(const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spec): ambient(amb),diffuse(diff),specular(spec){}
	LightColors(const glm::vec3& diff, const glm::vec3& spec) : ambient(0), diffuse(diff), specular(spec) {}

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

class PointLight : public GameObject, public Bindable {
public:
	PointLight(Shader* shader, const Transform& transInfo, const LightAttenuation& attenuation,
		const LightColors& colors);
	~PointLight();
	void addToShader();
	void localBind() override;
	void localUnbind() override;

protected:
	bool tryUpdateModelMatrix() override;

private:
	LightAttenuation attenuation_;
	LightColors light_;
	short shaderIndex_ = -1;
	static inline unsigned int count_ = 0;

};

class SpotLight : public GameObject, public Bindable {
public:
	SpotLight(Shader* shader, const Transform& transInfo, const LightColors& colors, const float angle, const float outerAngle);
	void addToShader();
	void localBind() override;
	void localUnbind() override;

protected:
	bool tryUpdateModelMatrix() override;

private:
	LightColors light_;
	float angle_, outerAngle_;
	short shaderIndex_ = -1;
	static inline unsigned int count_ = 0;


};