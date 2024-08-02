#pragma once
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Drawable.hpp"
#include "Bindable.hpp"
#include "Material.hpp"
#include "Constants.hpp"
class Model: public Drawable, public Bindable {
public:
	Model();
	Model(const std::string& path);

	Model(const std::vector<Mesh>& meshes);
	void draw() override;
	std::vector<Mesh> meshes;
	void localBind() override;
	void localUnbind() override;


private:
	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type);
};