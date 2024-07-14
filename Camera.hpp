#pragma once

#include <glm/glm.hpp>

class Camera {
public:
	Camera(const glm::vec3& position = {0.0,0.0,0.0}, const glm::vec3& front = { -1.0,0.0,0.0 }, const glm::vec3& up = { 0.0,1.0,0.0 });

private:
	glm::vec3 position_, front_, up_;
};