#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Transform.hpp"
#include "Shader.hpp"
#include "Bindable.hpp"
#include "Mesh.hpp"
#include "Drawable.hpp"
#include "Model.hpp"

class GameObject: public Transform {

public:
	GameObject(Shader* shader, const Transform& transInfo) : Transform(transInfo), shader_(shader) {}
	virtual ~GameObject() = default;
	void drawModel();
	Model* model;
protected:
	Shader* shader_;
};
