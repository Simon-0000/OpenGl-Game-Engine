#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Drawable.hpp"
#include "Bindable.hpp"

static std::unordered_map<GLenum, unsigned int> typeToSize{ 
	
	{GL_HALF_FLOAT, sizeof(float)/2},
	{GL_FLOAT, sizeof(float)},
	{GL_DOUBLE, sizeof(double)},
	{GL_BOOL, sizeof(bool)},
	{GL_INT, sizeof(int)},
	{GL_UNSIGNED_INT, sizeof(unsigned int)},
	{GL_SHORT, sizeof(short)},
};

struct AttributeDescriptor {
	GLenum type;
	unsigned int size;
};

struct BasicVertex {
	glm::vec3 positions;
	glm::vec2 TextureCoords;
	glm::vec3 normals;

};
class Mesh : public Drawable, public Bindable {
public:

	template<typename T>
	Mesh(const std::vector<T>& vertices, const std::vector<unsigned int>& indices, AttributeDescriptor* attributes, size_t attributesSize):
		Mesh(&vertices[0], sizeof(BasicVertex)* vertices.size(), &indices[0], sizeof(unsigned int)* indices.size(), attributes, attributesSize){
		
	}

	template<typename T>
	Mesh(const T* vertices, size_t verticesSize, const unsigned int* indices, size_t indicesSize, AttributeDescriptor* attributes, size_t attributesSize){
		//generate VAO(VBO,EBO) which VAO Also stores the vertex attributes
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		id_ = VAO;
		numberOfIndices_ = indicesSize / sizeof(unsigned int);
		unsigned int EBO;
		glGenBuffers(1, &EBO);

		unsigned int VBO;
		glGenBuffers(1, &VBO);


		//bind the VAO before the other two
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

		//vertex attributes
		std::vector<unsigned int> offsets = { 0 };
		for (unsigned int i = 0; i < attributesSize; ++i)
			offsets.push_back(offsets[offsets.size() - 1] + attributes[i].size * typeToSize[attributes[i].type]);

		for (unsigned int i = 0; i < attributesSize; ++i) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, attributes[i].size, attributes[i].type, GL_FALSE, offsets[offsets.size() - 1], (void*)offsets[i]);
		}
	}
	
	void draw() override;
	void localBind() override;
	void localUnbind() override;

private:
	unsigned int id_,numberOfIndices_;
};

class Cube :public Mesh{
public:
	Cube(float halfSideLength = 0.5f);
private:
	static Mesh createCube(float halfSideLength);


};

