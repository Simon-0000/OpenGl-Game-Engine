#pragma once
#include <glm/glm.hpp>
#include "Transform.hpp"
#include "Shader.hpp"
#include "Bindable.hpp"

class GameObject: public Transform, public Bindable {

public:
	GameObject(Shader* shader,const Transform& transInfo) : Transform(transInfo), shader_(shader) {}
	virtual ~GameObject() = default;
	virtual void draw() = 0;

	void setParent(GameObject* parent);
protected:
	Shader* shader_;
};