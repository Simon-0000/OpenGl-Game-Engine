#pragma once
#include <glm/glm.hpp>
#include "Bindable.hpp"

class RelativeGameObject: public Bindable {
public:
	RelativeGameObject(const glm::vec3& position);
	void attachToParent(Bindable* parent);
protected:
	glm::vec3 position_;
	Bindable* parent;
};