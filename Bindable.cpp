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
		if (childCounter == 0)//put the child before the brother in the linked list
		{
			auto brother = relative_;
			relative_ = child;
			child = brother;
		}
		relative_->linkBrother(child);
	}
	else
		relative_ = child;

	++childCounter;
}
void Bindable::linkBrother(Bindable* brother) {
	if (relative_)
		relative_->linkBrother(brother);
	else
		relative_ = brother;
}

void Bindable::unlinkBrother() {
	Bindable* brother = relative_;
	for (int i = 0; i < childCounter - 1; ++i) {
		brother = brother->relative_;
	}
	brother->relative_ = nullptr;
}