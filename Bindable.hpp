#pragma once
#include <iostream>
#include <vector>

class Bindable {
public:
	Bindable() = default;
	virtual ~Bindable() = default;

	virtual void localBind() = 0;
	void bind();
	void linkChild(Bindable* child);
private:
	std::vector<Bindable*> childs_;


};