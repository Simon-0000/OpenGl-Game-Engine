#include "GameObject.hpp"

void GameObject::draw()
{
	shader_->use();
	shader_->setMat4f("uModel", &getUpdatedModelMatrix());
	if (model) {
		model->draw();
	}
}

void GameObject::localDraw()
{
	shader_->use();
	shader_->setMat4f("uModel", &getUpdatedModelMatrix());
	if (model) {
		model->localDraw();
	}
}

void GameObject::drawWithOutline(const std::vector<GameObject*>& objects,const glm::vec3& color,float outlineWidthPercentage)
{
	static Shader& unlitShader = Shader::tryCreateShader(ShaderNames::unlitShaderVs, ShaderNames::unlitShaderFs);
	unlitShader.useThenSetVec3f("uLightColor", &color);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glClear(GL_STENCIL_BUFFER_BIT);
	

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	for (auto& obj : objects)
		obj->draw();


	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	for (auto& obj : objects) {
		auto temp = obj->shader_;
		obj->shader_ = &unlitShader;
		obj->scale(glm::vec3( outlineWidthPercentage ));
		obj->localDraw();
		obj->scale(glm::vec3(-outlineWidthPercentage));
		obj->shader_ = temp;
	}

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xff);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);

}
