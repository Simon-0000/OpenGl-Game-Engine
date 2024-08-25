#pragma once
#include <iostream>
#include <vector>
#include "Transform.hpp"
#include "Shader.hpp"
#include "Bindable.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera : public Transform, public Bindable {
public:
	Camera(const Transform& transform, float fovDeg, float aspectRatio, float renderDistance, float minRenderDistance = 0.1f);
	void linkShader(Shader* shader);
	void linkShaders(std::vector<Shader*> shaders);
	void localBind() override;
	void localUnbind() override;
	bool update();
	float fovRad, aspectRatio, renderDistance, minRenderDistance;
protected:
	bool tryUpdateModelMatrix() override;
private:
	std::vector<Shader*> shaders_;
};
