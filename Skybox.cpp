#include "Skybox.hpp"

Skybox::Skybox(Cubemap* texture):Mesh(createSkyboxMesh()) {
	linkChild(texture);
}

void Skybox::draw()
{
	static Shader& cubemapShader = Shader::tryCreateShader(ShaderNames::skyboxShaderVs, ShaderNames::skyboxShaderFs);
	glDepthMask(GL_FALSE);
	cubemapShader.use();
	Mesh::draw();
	glDepthMask(GL_TRUE);
}

Mesh Skybox::createSkyboxMesh()
{
	std::vector<glm::vec3> vertices = {
		{-1.0f,  1.0f, -1.0f},
		{-1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f,  1.0f, -1.0f},
		
		{-1.0f, -1.0f,  1.0f},
		{-1.0f, -1.0f, -1.0f},
		{-1.0f,  1.0f, -1.0f},
		{-1.0f,  1.0f,  1.0f},
		
		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f, -1.0f},
		
		{-1.0f, -1.0f,  1.0f},
		{-1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f, -1.0f,  1.0f},
		
		{-1.0f,  1.0f, -1.0f},
		{ 1.0f,  1.0f, -1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{-1.0f,  1.0f,  1.0f},
		
		{-1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{-1.0f, -1.0f,  1.0f}

	}; 

	std::vector<unsigned int> indices = {
		  0, 1, 2, 2, 3, 0,
		  4, 5, 6, 6, 7, 4,
		  8, 9, 10, 10, 11, 8,
		  12, 13, 14, 14, 15, 12, 
		  16, 17, 18, 18, 19, 16,
		  22, 21, 20, 20, 23, 22  
	};

	AttributeDescriptor attributes[] = { { GL_FLOAT,3 } };
	return Mesh(vertices, indices, attributes, 1);
}
