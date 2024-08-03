#pragma once
#include <iostream>
#include <unordered_set>

class Bindable {
public:
	Bindable() = default;
	virtual ~Bindable() = default;

	virtual void localBind() = 0;
	virtual void localUnbind() = 0;
	void bind();
	void unbind();

	void linkChild(Bindable* child);
	void unlinkChild(Bindable* child);
	
private:
	void linkChild(Bindable* child,std::unordered_set<Bindable*>& parents);

	Bindable* sibling_ = nullptr;
	Bindable* child_ = nullptr;
};
