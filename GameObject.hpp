#pragma once
#include <glm/glm.hpp>
#include "Transform.hpp"

class GameObject: public Transform {
public:
	GameObject(glm::vec3 position_, glm::vec3 rotation);
	void attachToParent(GameObject* parent);
};