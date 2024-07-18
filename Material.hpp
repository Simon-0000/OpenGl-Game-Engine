#pragma once
#include <glm/glm.hpp>
#include "Texture.hpp"
#include "Shader.hpp"
class Material: public Texture{
public:
	Material(const char* imagePath, Shader* shader, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess = 16);

	void localBind() override;

	glm::vec3 ambient, diffuse, specular;
	float shininess;
private:
	Shader* shader_;

};