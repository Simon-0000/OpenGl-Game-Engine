#pragma once
#include <glad/glad.h>
#include "Bindable.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
class FrameBuffer : public Bindable{
public:
	FrameBuffer(Shader* shader);
	~FrameBuffer();
	void localBind() override;
	void localUnbind() override;
	Texture texture;
private:
	unsigned int id_;
};