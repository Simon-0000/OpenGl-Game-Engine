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
	template<typename T>
	void linkUniform(const char* name, std::function<T(Camera*)> uniformCalculation)
	{
		uniformsCalculator_[name] = [=,value = T{}](Shader* shader, bool calculate) mutable {
			//static bool recalculate;
			//if (calculate) {
			//	recalculate = true;
			//	return;
			//}
			//if (recalculate) {
			//	//recalculate = false;
			//	value = uniformCalculation(this);
			//}
			T a = uniformCalculation(this);
			shader->use();
			shader->setUniform(name, &a);
		};
	}

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
