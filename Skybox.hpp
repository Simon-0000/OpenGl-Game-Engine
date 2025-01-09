#pragma once
#include "Mesh.hpp"
#include "Cubemap.hpp"
#include "Shader.hpp"
#include "Constants.hpp"
#include "GameObject.hpp"
class Skybox : public GameObject {
public:
	Skybox(Cubemap* texture);
	void draw() override;

private:
	Cubemap* texture_;
	static Mesh createSkyboxMesh();

};