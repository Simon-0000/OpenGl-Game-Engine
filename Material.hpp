#pragma once
#include <glm/glm.hpp>
#include "Texture.hpp"
#include "Shader.hpp"
class Material: public Bindable{
public:
	Material(const char* diffuseImagePath,const char* specularImagePath, Shader* shader, float shininess = 16);

	void localBind() override;
	void localUnbind() override;

	float shininess;
private:
	Shader* shader_;
	Texture diffuseImage_, specularImage_;
};