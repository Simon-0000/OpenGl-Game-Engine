#include "Camera.hpp"

Camera::Camera(const Transform& transform, float fovDeg, float aspectRatio, float renderDistance, float minRenderDistance):
	Transform(transform), uniformsCalculator_({}), shadersUniforms_({}), fovRad(glm::radians(fovDeg)), aspectRatio(aspectRatio), renderDistance(renderDistance), minRenderDistance(minRenderDistance)
{
}

void Camera::linkShader(Shader* shader,std::unordered_set<const char*> uniforms)
{
	shadersUniforms_.push_back({shader,uniforms});
}

void Camera::localBind()
{
	glm::mat4 projection;
	projection = glm::perspective(fovRad, aspectRatio, minRenderDistance, renderDistance);
	for (auto& shaderUniforms : shadersUniforms_)
		shaderUniforms.first->useThenSetMat4f("uProjection", &projection);
}

void Camera::localUnbind()
{
	assert(false);
}

bool Camera::update()
{
	return tryUpdateModelMatrix();
}

bool Camera::tryUpdateModelMatrix() {

	if (Transform::tryUpdateModelMatrix()) {
		//for (auto uniformsFunc : uniformsCalculator_) {
		//	uniformsFunc.second(nullptr, true);
		//}
		for (auto& shaderUniforms : shadersUniforms_)
		{
			for (auto& uniformName : shaderUniforms.second) {
				auto it = uniformsCalculator_.find(uniformName);
				if (it != uniformsCalculator_.end())
					(*it).second(shaderUniforms.first, false);
			}
		}
		return true;
	}
	return false;
}