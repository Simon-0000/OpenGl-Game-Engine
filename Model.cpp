#include "Model.hpp"

Model::Model(): meshes({})
{
}

Model::Model(const char* path): meshes({})
{
	loadModel(path);
}

Model::Model(const std::vector<Mesh>& meshes):meshes(meshes)
{
}

void Model::draw()
{
	for (auto& mesh : meshes)
		mesh.draw();
}
void Model::localDraw()
{
	for (auto& mesh : meshes)
		mesh.localDraw();
}

void Model::loadModel(const char* path)
{
	Assimp::Importer importer;
	
	const aiScene* scene = importer.ReadFile( path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);//TODO add aiProcess_GenNormals and aiProcess_OptimizeMeshes
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw std::runtime_error("Could not load model, directory is invalid");
	}
	auto directory = std::string(path);
	auto pos = directory.rfind("/");
	if (pos != directory.npos)
	{
		processNode(directory.substr(0, pos + 1).c_str(), scene->mRootNode, scene);
	}
	else {
		processNode(directory.c_str(), scene->mRootNode, scene);
	}
}

void Model::processNode(const char* path, aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(path, mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
		processNode(path, node->mChildren[i], scene);
}

Mesh Model::processMesh(const char* path, aiMesh* mesh, const aiScene* scene) {
	std::vector<BasicVertex> vertices;
	std::vector<unsigned int> indices;
	AttributeDescriptor attributes[] = { {GL_FLOAT,3 },{GL_FLOAT,2},{GL_FLOAT,3} };

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		glm::vec2 uvs = {};
		glm::vec3 normals = {};
		if (mesh->mTextureCoords[0])
			uvs = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
		
		if (mesh->HasNormals())
			normals = { mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z };

		BasicVertex vertex{
			{mesh->mVertices[i].x  ,mesh->mVertices[i].y, mesh->mVertices[i].z},
			uvs,
			normals
		};
		vertices.push_back(vertex);

	}
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	//TODO finish material
	//vector<Texture*> 
	Mesh mMesh = Mesh(vertices, indices, attributes, 3);
	mMesh.linkChild(loadMaterialTextures(path, material, aiTextureType_DIFFUSE, 0, "uMaterial.diffuse"));
	mMesh.linkChild(loadMaterialTextures(path, material, aiTextureType_SPECULAR, 0, "uMaterial.specular"));

	return mMesh;
}

Texture* Model::loadMaterialTextures(const char* path, aiMaterial* mat, aiTextureType type, const unsigned int textureUnit, const char* TextureName)
{
	Texture* texture = nullptr;
	if(mat->GetTextureCount(type) > 0)
	{
		aiString subPath;
		mat->GetTexture(type, 0, &subPath);
		texture = &Texture::tryCreateTexture((std::string(path) + subPath.C_Str()).c_str(), &Shader::tryCreateShader("shader.vs", "shader.fs"), textureUnit, TextureName);
	}
	return texture;
}