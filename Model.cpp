#include "Model.hpp"

Model::Model(): meshes({})
{
}

Model::Model(const std::vector<Mesh>& meshes):meshes(meshes)
{
}

void Model::draw()
{
	for (auto& mesh : meshes)
		mesh.draw();
}

void Model::localBind()
{
}

void Model::localUnbind()
{
}
