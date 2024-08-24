#pragma once
#include <glad/glad.h>
#include "Bindable.hpp"
#include "Texture.hpp"
class FrameBuffer : public Bindable{
public:
	FrameBuffer();
	~FrameBuffer();
	void localBind() override;
	void localUnbind() override;
	void getId();
private:
	Texture texture_;
	unsigned int id_;
};