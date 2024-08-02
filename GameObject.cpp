#include "GameObject.hpp"

void GameObject::drawModel()
{
	shader_->useThenSetMat4f("uModel", &getUpdatedModelMatrix());
	model->draw();
}
