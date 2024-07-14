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
	void linkBrother(Bindable* brother);

	void unlinkBrother();

private:
	Bindable* relative_ = nullptr;
	int childCounter = 0;
};