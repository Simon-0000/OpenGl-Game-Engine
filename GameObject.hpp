#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "Transform.hpp"
#include "Shader.hpp"
#include "Bindable.hpp"
#include "Mesh.hpp"

class GameObject: public Transform {

public:
	GameObject(Shader* shader, const Transform& transInfo) : Transform(transInfo), shader_(shader) {}
	virtual ~GameObject() = default;
	void drawMeshes();
	std::vector<Mesh*> meshes = {};
protected:
	Shader* shader_;
};