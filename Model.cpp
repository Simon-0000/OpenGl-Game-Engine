#include "Model.hpp"

Model::Model(): meshes({})
{
}

Model::Model(const std::string& path): meshes({})
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

void Model::localBind()
{
}

void Model::localUnbind()
{
}

void Model::loadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);//TODO add aiProcess_GenNormals and aiProcess_OptimizeMeshes
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw std::runtime_error("Could not load model, directory is invalid");
	}
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (int i = 0; i < node->mNumChildren; ++i)
		processNode(node->mChildren[i], scene);
}
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<BasicVertex> vertices;
	std::vector<unsigned int> indices;
	AttributeDescriptor attributes[] = { {GL_FLOAT,3 },{GL_FLOAT,2},{GL_FLOAT,3} };

	for (int i = 0; i < mesh->mNumVertices; ++i)
	{
		BasicVertex vertex{
			{ mesh->mVertices[i].x  ,mesh->mVertices[i].y, mesh->mVertices[i].z},
			{},//{mesh->mTextureCoords[i]->x,mesh->mTextureCoords[i]->y},
			{mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z}
		};
		vertices.push_back(vertex);
	}
	for (int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[i]);
	}
	return Mesh(vertices, indices, attributes,3);
}
