#include "GameObject.hpp"

//GameObject::GameObject() {
//
//}
void GameObject::attachToParent(GameObject* parent) {
	if (parent_)
		parent_->unlinkChild(this);
	parent_ = parent;
	parent_->linkChild(this);
}