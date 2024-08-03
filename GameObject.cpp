#include "GameObject.hpp"

void GameObject::draw()
{
	shader_->useThenSetMat4f("uModel", &getUpdatedModelMatrix());
	if (model) {
		model->draw();
	}
}
