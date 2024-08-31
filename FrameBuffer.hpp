#pragma once
#include <glad/glad.h>
#include "Bindable.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

class FrameBuffer : public Bindable{
public:
	FrameBuffer();
	~FrameBuffer();
	void renderFrameBuffer(Renderer& renderer, Shader& customShader);
	void localBind() override;
	void localUnbind() override;
	Texture texture;
private:
	unsigned int id_;
};