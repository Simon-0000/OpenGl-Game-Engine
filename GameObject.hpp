#pragma once
#include <glm/glm.hpp>
#include "Transform.hpp"
#include "Shader.hpp"

class GameObject: public Transform {

public:
	GameObject(Shader* shader,const Transform& transInfo) : Transform(transInfo), shader_(shader) {}
	virtual ~GameObject() = default;
	virtual void draw() = 0;
protected:
	Shader* shader_;
};