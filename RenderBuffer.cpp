#include "RenderBuffer.hpp"

RenderBuffer::RenderBuffer(FrameBuffer& fBuffer)
{
	glGenRenderbuffers(1, &id_);
	localBind();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT);
	localUnbind();
	fBuffer.localBind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id_);
}

RenderBuffer::~RenderBuffer()
{
}

void RenderBuffer::localBind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, id_);
}

void RenderBuffer::localUnbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
