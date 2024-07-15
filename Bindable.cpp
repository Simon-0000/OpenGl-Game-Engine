#include "Bindable.hpp"

void Bindable::bind() {
	localBind();
	if(relative_)
		relative_->bind();
}
void Bindable::unbind() {
	localUnbind();
	if (relative_)
		relative_->unbind();
}

void Bindable::linkChild(Bindable* child) {
	if (relative_) {
		if (childCounter_ == 0)//put the child before the brother in the linked list
		{
			auto brother = relative_;
			relative_ = child;
			child = brother;
		}
		relative_->linkBrother(child);
	}
	else
		relative_ = child;

	childCounter_ += child->childCounter_;
}
void Bindable::linkBrother(Bindable* brother) {
	if (relative_)
		relative_->linkBrother(brother);
	else
		relative_ = brother;
}


void Bindable::unlinkChild(Bindable* child) {

	Bindable*& relative = relative_;
	while (relative != child) {
		relative = relative->relative_;
	}
	if (relative) {
		relative = relative->getBrother();
		childCounter_ -= relative->childCounter_;
	}
}
Bindable*& Bindable::getBrother() {
	Bindable*& brother = relative_;
	for (int i = 0; i < childCounter_; ++i)
		brother = brother->relative_;
	return brother;
}

bool Bindable::hasNoRelative()
{
	return relative_ == nullptr;
}

