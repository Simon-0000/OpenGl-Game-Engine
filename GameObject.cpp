#include "GameObject.hpp"

void GameObject::setParent(GameObject* parent) {

	Transform::setParent(parent);
	//TODO unbind parent here
	parent->linkChild(this);
}