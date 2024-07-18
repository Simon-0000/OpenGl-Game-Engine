#pragma once
#include <glm/glm.hpp>
#include "Texture.hpp"
#include "Shader.hpp"
class Material: public Bindable{
public:
	Material(const char* diffuseImagePath,const char* specularImagePath, Shader* shader, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess = 16);

	void localBind() override;
	void localUnbind() override;

	glm::vec3 ambient, diffuse, specular;
	float shininess;
private:
	Shader* shader_;
	Texture diffuseImage_, specularImage_;
};