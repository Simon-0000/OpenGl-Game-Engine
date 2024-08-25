#include "FrameBuffer.hpp"

FrameBuffer::FrameBuffer():texture_()
{
	glGenFramebuffers(1, &id_);
	localBind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_.getId(), 0);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1,&id_);
}

void FrameBuffer::localBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, id_);
}

void FrameBuffer::localUnbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
