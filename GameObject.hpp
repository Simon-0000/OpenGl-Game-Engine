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

class GameObject: public Transform, public Drawable {

public:
	GameObject(Shader* shader, const Transform& transInfo, Model* modelToUse = nullptr) : Transform(transInfo), shader_(shader), model(modelToUse){}
	virtual ~GameObject() = default;
	void draw() override;
	void localDraw();

	static void drawWithOutline(const std::vector<GameObject*>& objects, const glm::vec3& color, float outlineWidthPercentage = 0.05f);

	Shader* shader_;
	Model* model;


protected:
};
