#include "Primitive.hpp"

void Primitive::localBind() {
	glBindVertexArray(id_);
}

void Primitive::localUnbind() {
	glBindVertexArray(0);
}

unsigned int Primitive::getNumberOfIndices() {
	return numberOfIndices_;
}


Primitive Primitive::cube() {
	//init 
	Vertex baseVertices[] = {
		{{ 0.5f,  0.5f, 0.5f},	 {1.0f, 1.0f}},   // top right
		{{ 0.5f, -0.5f, 0.5f},	 {1.0f, 0.0f}},   // bottom right
		{{-0.5f, -0.5f, 0.5f},	 {0.0f, 0.0f}},   // bottom left
		{{-0.5f,  0.5f, 0.5f},	 {0.0f, 1.0f}}    // top left 
	};
	
	std::vector<Vertex> vertices;
	glm::mat4 rotationMatrix = glm::mat4(1);
	for (int j = 0; j < 4; ++j) {
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		for (int i = 0; i < 4; ++i) {
			glm::vec4 next = rotationMatrix * glm::vec4(baseVertices[i].positions, 1.0f);
			vertices.push_back(Vertex(next, baseVertices[i].TextureCoords));
		}
	}
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	for (int j = 0; j < 2; ++j) {
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		for (int i = 0; i < 4; ++i) {
			glm::vec4 next = rotationMatrix * glm::vec4(baseVertices[i].positions, 1.0f);
			vertices.push_back(Vertex(next, baseVertices[i].TextureCoords));
		}
	}

	unsigned int baseIndices[] = {
		0, 1, 2,   // first triangle
		2, 3, 0    // second triangle
	};
	std::vector<unsigned int> indices;//(baseIndices, baseIndices + sizeof(baseIndices)/sizeof(unsigned int));
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			indices.push_back(baseIndices[j] + 4 * i);
		}
	}

	AttributeDescriptor attributes[] = { {GL_FLOAT,3 },{GL_FLOAT,2} };
	return Primitive(&vertices[0], sizeof(Vertex) * vertices.size(), &indices[0], sizeof(unsigned int) * indices.size(), attributes, 3);
}