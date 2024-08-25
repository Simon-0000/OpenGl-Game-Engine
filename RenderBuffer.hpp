#pragma once
#include <glad/glad.h>
#include "Bindable.hpp"
#include "Constants.hpp"
class RenderBuffer :public Bindable {
public:
	RenderBuffer();
	~RenderBuffer();
	void localBind() override;
	void localUnbind() override;
private:
	unsigned int id_;

};