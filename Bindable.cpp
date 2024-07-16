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
	if (child_) {
		child_->linkChild(child);
	}
	else
		child_ = child;
}


void Bindable::unlinkChild(Bindable* child) {

	Bindable*& relative = child_;
	while (relative != child)
		if (!(relative = relative->sibling_))
			return;
	relative = relative->sibling_;
}


