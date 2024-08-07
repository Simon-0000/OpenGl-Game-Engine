#include "GameObject.hpp"

void GameObject::draw()
{
	shader_->useThenSetMat4f("uModel", &getUpdatedModelMatrix());
	if (model) {
		model->draw();
	}
}

void GameObject::localDraw()
{
	shader_->useThenSetMat4f("uModel", &getUpdatedModelMatrix());
	if (model) {
		model->localDraw();
	}
}

void GameObject::drawWithOutline(const std::vector<GameObject*>& objects,const glm::vec3& color,float outlineWidthPercentage)
{
	LightShader::unlitShader().useThenSetVec3f("uLightColor",&color);
	//glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_ALWAYS, 1, 0xFF);
	//glStencilMask(0xFF);

	for (auto& obj : objects) {
		auto temp = obj->shader_;
		obj->shader_ = &LightShader::unlitShader();
		obj->localDraw();
		obj->shader_ = temp;

	}

	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilMask(0x00);
	//glDisable(GL_DEPTH_TEST);
	//for (auto& obj : objects) {
	//	auto temp = obj->shader_;
	//	obj->shader_ = &LightShader::unlitShader();
	//	obj->scale(glm::vec3( outlineWidthPercentage ));
	//	obj->localDraw();
	//	obj->scale(glm::vec3(-outlineWidthPercentage));
	//	obj->shader_ = temp;
	//}

	//glStencilMask(0xFF);
	//glStencilFunc(GL_ALWAYS, 1, 0xff);
	////glDisable(GL_STENCIL_TEST);
	//glEnable(GL_DEPTH_TEST);

}
