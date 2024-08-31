#include "FrameBuffer.hpp"

FrameBuffer::FrameBuffer():texture()
{
	glGenFramebuffers(1, &id_);
	localBind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.getId(), 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::logic_error("Framebuffer is incomplete");
	localUnbind();
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1,&id_);
}

void FrameBuffer::renderFrameBuffer(Renderer& renderer, Shader& customShader)
{
	static Mesh screenQuad = Quad2d(1.0f, 1.0f);	// first pass
	bind();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);

	renderer.renderBuffers();
	localUnbind();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	customShader.use();
	glDisable(GL_DEPTH_TEST);
	texture.bind();
	screenQuad.draw();
}

void FrameBuffer::localBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, id_);
}

void FrameBuffer::localUnbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
