#pragma once
#include <glad/glad.h>
#include "Bindable.hpp"
#include "Constants.hpp"
#include "FrameBuffer.hpp"
class RenderBuffer :public Bindable {
public:
	RenderBuffer(FrameBuffer& fBuffer);
	~RenderBuffer();
	void localBind() override;
	void localUnbind() override;
private:
	unsigned int id_;

};