#pragma once
#include <iostream>
#include <vector>

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
	Bindable* sibling_ = nullptr;
	Bindable* child_ = nullptr;
};