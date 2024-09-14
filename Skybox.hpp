#pragma once
#include "Mesh.hpp"
#include "Cubemap.hpp"
#include "Shader.hpp"
#include "Constants.hpp"
class Skybox : public Mesh {
public:
	Skybox(Cubemap* texture);
	void draw() override;

private:
	static Mesh createSkyboxMesh();

};