#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "Transform.hpp"
#include "Shader.hpp"
#include "Bindable.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera : public Transform, public Bindable {
public:
	Camera(const Transform& transform, float fovDeg, float aspectRatio, float renderDistance, float minRenderDistance = 0.1f);
	void linkShader(Shader* shader,std::unordered_set<const char*> uniforms);
	void linkUniform(const char* name, std::function<glm::mat4(Camera*)> matrixUniform);
	void linkUniform(const char* name, std::function<glm::vec3(Camera*)> vectorUniform);

	void localBind() override;
	void localUnbind() override;
	bool update();
	float fovRad, aspectRatio, renderDistance, minRenderDistance;
protected:
	bool tryUpdateModelMatrix() override;
private:
	std::unordered_map<const char*, std::function<void (Shader*, bool)>> uniformsCalculator_;
	std::vector<std::pair<Shader*,std::unordered_set<const char*>>> shadersUniforms_;
};
