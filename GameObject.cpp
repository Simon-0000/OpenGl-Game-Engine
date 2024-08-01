#include "GameObject.hpp"

void GameObject::drawMeshes()
{
	shader_->useThenSetMat4f("uModel", &getUpdatedModelMatrix());
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->draw();
}
