#include "Camera.hpp"

Camera::Camera(const Transform& transform, float fovDeg, float aspectRatio, float renderDistance, float minRenderDistance):
	Transform(transform), shaders_({}), fovRad(glm::radians(fovDeg)), aspectRatio(aspectRatio), renderDistance(renderDistance), minRenderDistance(minRenderDistance)
{
}

void Camera::linkShader(Shader* shader)
{
	shaders_.push_back(shader);
}

void Camera::linkShaders(std::vector<Shader*> shaders)
{
	for (auto& shader : shaders)
		linkShader(shader);
}

void Camera::localBind()
{
	glm::mat4 projection;
	projection = glm::perspective(fovRad, aspectRatio, minRenderDistance, renderDistance);
	for (auto& shader : shaders_)
		shader->useThenSetMat4f("uProjection", &projection);
}

void Camera::localUnbind()
{
}

void Camera::update()
{
	tryUpdateModelMatrix();
}

bool Camera::tryUpdateModelMatrix() {
	if (Transform::tryUpdateModelMatrix()) {
		glm::mat4 view;
		view = glm::lookAt(getPosition(), getPosition() + getForward(), {0,1.0f,0});
		for (auto& shader : shaders_)
			shader->useThenSetMat4f("uView", &view);

		return true;
	}
	return false;
}