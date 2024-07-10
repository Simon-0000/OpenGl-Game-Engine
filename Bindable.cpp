#include "Bindable.hpp"

void Bindable::bind() {
	localBind();
	if(child_)
		child_->bind();
}
void Bindable::unbind() {
	localUnbind();
	if (child_)
		child_->unbind();
}

void Bindable::linkChild(Bindable* child) {
	if (child_)
		child_->linkChild(child);
	else
		child_ = child;
}