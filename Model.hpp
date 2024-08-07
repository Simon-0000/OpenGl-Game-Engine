#pragma once
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Drawable.hpp"
#include "Bindable.hpp"
#include "Material.hpp"
#include "Constants.hpp"
class Model: public Drawable {
public:
	Model();
	Model(const char* path);

	Model(const std::vector<Mesh>& meshes);
	void draw() override;
	void localDraw();

	std::vector<Mesh> meshes;


private:
	void loadModel(const char* path);
	void processNode(const char* directory, aiNode* node, const aiScene* scene);
	Mesh processMesh(const char* directory, aiMesh* mesh, const aiScene* scene);
	Texture* loadMaterialTextures(const char* directory, aiMaterial* mat, aiTextureType type, const unsigned int textureUnit, const char* TextureName);
};