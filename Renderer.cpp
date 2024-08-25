#include "Renderer.hpp"

Renderer::Renderer(Camera* camera) : referenceCamera(camera), opaqueBuffer_(), transparentBuffer_()
{

}
void Renderer::addToOpaqueBuffer(GameObject* obj)
{
	opaqueBuffer_.push_back(obj);
}

void Renderer::addToTransparentBuffer(GameObject* obj)
{
	transparentBuffer_.push_back(obj);
}

void Renderer::renderBuffers()
{
	for (auto& obj : opaqueBuffer_)
		obj->draw();
	if (transparentBuffer_.size() > 0) {
		for (auto& obj : transparentBuffer_)
			obj->draw();
	}
}

void Renderer::clearBuffers()
{
	opaqueBuffer_.clear();
	transparentBuffer_.clear();
}

void Renderer::sortBuffers()
{
	std::sort(opaqueBuffer_.begin(), opaqueBuffer_.end(), *this);
	sortTransparentBuffer();
}

void Renderer::sortTransparentBuffer()
{
	std::sort(transparentBuffer_.begin(), transparentBuffer_.end(), *this);
}

bool Renderer::operator()(GameObject* obj1, GameObject* obj2)
{

	auto posCam = referenceCamera->getGlobalPosition();
	return  glm::distance2(obj1->getGlobalPosition(), posCam) > glm::distance2(obj2->getGlobalPosition(), posCam);
}

Renderer::Iterator Renderer::begin()
{
	return Iterator(this,0);
}
Renderer::Iterator Renderer::end()
{
	return Iterator(this, opaqueBuffer_.size() + transparentBuffer_.size());
}


