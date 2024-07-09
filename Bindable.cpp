#include "Bindable.hpp"

void Bindable::bind() {
	localBind();
	for (auto child : childs_)
		child->bind();
}
void Bindable::linkChild(Bindable* child) {
	childs_.push_back(child);
}