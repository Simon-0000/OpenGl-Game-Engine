#include "Camera.hpp"

Camera::Camera(const Transform& transform, float fovDeg, float aspectRatio, float renderDistance, float minRenderDistance):
	Transform(transform), uniformsCalculator_({}), shadersUniforms_({}), fovRad(glm::radians(fovDeg)), aspectRatio(aspectRatio), renderDistance(renderDistance), minRenderDistance(minRenderDistance)
{
}

void Camera::linkShader(Shader* shader,std::unordered_set<const char*> uniforms)
{
	shadersUniforms_.push_back({shader,uniforms});
}

void Camera::linkUniform(const char* name, std::function<glm::mat4(Camera*)> matrixUniform)
{
	uniformsCalculator_[name] = [=](Shader* shader, bool calculate) {
		static glm::mat4 value;
		static bool recalculate;
		if (calculate) {
			recalculate = true;
			return;
		}
		if (recalculate) {
			recalculate = false;
			value = matrixUniform(this);
		}

		shader->useThenSetMat4f(name, &value);
	};
}
void Camera::linkUniform(const char* name, std::function<glm::vec3(Camera*)> vectorUniform) {//TODO remove this dupplicate function later on
	uniformsCalculator_[name] = [=](Shader* shader, bool calculate) {
		static glm::vec3 value;
		static bool recalculate;
		if (calculate) {
			recalculate = true;
			return;
		}
		if (recalculate) {
			recalculate = false;
			value = vectorUniform(this);
		}

		shader->useThenSetVec3f(name, &value);
	};
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
	for (auto uniformsFunc : uniformsCalculator_) {
		uniformsFunc.second(nullptr, true);
	}
	if (Transform::tryUpdateModelMatrix()) {
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