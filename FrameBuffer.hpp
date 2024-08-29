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
	Texture texture;

private:
	unsigned int id_;
};