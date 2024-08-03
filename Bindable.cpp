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

void Bindable::linkChild(Bindable* child) 
{
	std::unordered_set<Bindable*> parents = { this };
	linkChild(child, parents);

}


void Bindable::unlinkChild(Bindable* child) {
	Bindable*& relative = child_;
	while (relative != child)
		if (!(relative = relative->sibling_))
			return;
	relative = relative->sibling_;
}

void Bindable::linkChild(Bindable* child, std::unordered_set<Bindable*>& parents)
{
	if (child_) {
		if (parents.find(child_) == parents.end())//to prevent infinite loop (linking back to itself)
			return;
		parents.insert(child_);
		child_->linkChild(child,parents);
	}
	else
		child_ = child;
}



