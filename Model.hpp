#pragma once
#include <iostream>
#include <vector>
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Drawable.hpp"
#include "Bindable.hpp"
 
class Model: public Drawable, public Bindable {
public:
	Model();
	Model(const std::vector<Mesh>& meshes);
	void draw() override;
	std::vector<Mesh> meshes;
	void localBind() override;
	void localUnbind() override;
};