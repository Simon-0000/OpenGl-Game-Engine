#include "Mesh.hpp"

void Mesh::localBind() {
	glBindVertexArray(id_);
}

void Mesh::localUnbind() {
	glBindVertexArray(0);
}

void Mesh::draw() {
	bind();//assume that shader has been set up previously 
	localDraw();
}

void Mesh::localDraw() {
	glDrawElements(GL_TRIANGLES, numberOfIndices_, GL_UNSIGNED_INT, 0);
}



Cube::Cube(float halfSideLength) : Mesh(createCube(halfSideLength))
{

}
Mesh Cube::createCube(float halfSideLength) {
	std::vector<BasicVertex> vertices = {
		{{ halfSideLength, -halfSideLength,  -halfSideLength}, {0.0f,0.0f},  {0.0f,  0.0f,  -1.0f}},
		{{-halfSideLength, -halfSideLength,  -halfSideLength}, {1.0f,0.0f},  {0.0f,  0.0f,  -1.0f}},
		{{-halfSideLength,  halfSideLength,  -halfSideLength}, {1.0f,1.0f},  {0.0f,  0.0f,  -1.0f}},
		{{ halfSideLength,  halfSideLength,  -halfSideLength}, {0.0f,1.0f},  {0.0f,  0.0f,  -1.0f}},

		{{-halfSideLength, -halfSideLength,  halfSideLength}, {0.0f,0.0f},  {0.0f,  0.0f,  1.0f}},
		{{ halfSideLength, -halfSideLength,  halfSideLength}, {1.0f,0.0f},  {0.0f,  0.0f,  1.0f}},
		{{ halfSideLength,  halfSideLength,  halfSideLength}, {1.0f,1.0f},  {0.0f,  0.0f,  1.0f}},
		{{-halfSideLength,  halfSideLength,  halfSideLength}, {0.0f,1.0f},  {0.0f,  0.0f,  1.0f}},

		{{-halfSideLength,  halfSideLength,  halfSideLength}, {1.0f,1.0f}, {-1.0f,  0.0f,  0.0f}},
		{{-halfSideLength,  halfSideLength, -halfSideLength}, {0.0f,1.0f}, {-1.0f,  0.0f,  0.0f}},
		{{-halfSideLength, -halfSideLength, -halfSideLength}, {0.0f,0.0f}, {-1.0f,  0.0f,  0.0f}},
		{{-halfSideLength, -halfSideLength,  halfSideLength}, {1.0f,0.0f}, {-1.0f,  0.0f,  0.0f}},

		{{ halfSideLength,  halfSideLength, -halfSideLength}, {1.0f,1.0f}, {1.0f,  0.0f,  0.0f}},
		{{ halfSideLength,  halfSideLength,  halfSideLength}, {0.0f,1.0f}, {1.0f,  0.0f,  0.0f}},
		{{ halfSideLength, -halfSideLength,  halfSideLength}, {0.0f,0.0f}, {1.0f,  0.0f,  0.0f}},
		{{ halfSideLength, -halfSideLength, -halfSideLength}, {1.0f,0.0f}, {1.0f,  0.0f,  0.0f}},

		{{-halfSideLength, -halfSideLength, -halfSideLength}, {0.0f,0.0f}, {0.0f, -1.0f,  0.0f}},
		{{ halfSideLength, -halfSideLength, -halfSideLength}, {1.0f,0.0f}, {0.0f, -1.0f,  0.0f}},
		{{ halfSideLength, -halfSideLength,  halfSideLength}, {1.0f,1.0f}, {0.0f, -1.0f,  0.0f}},
		{{-halfSideLength, -halfSideLength,  halfSideLength}, {0.0f,1.0f}, {0.0f, -1.0f,  0.0f}},

		{{-halfSideLength,  halfSideLength, -halfSideLength}, {0.0f,1.0f}, {0.0f,  1.0f,  0.0f}},
		{{-halfSideLength,  halfSideLength,  halfSideLength}, {0.0f,0.0f}, {0.0f,  1.0f,  0.0f}},
		{{ halfSideLength,  halfSideLength,  halfSideLength}, {1.0f,0.0f}, {0.0f,  1.0f,  0.0f}},
		{{ halfSideLength,  halfSideLength, -halfSideLength}, {1.0f,1.0f}, {0.0f,  1.0f,  0.0f}},
	};

	unsigned int baseIndices[] = {
		0, 1, 2,
		2, 3, 0
	};
	std::vector<unsigned int> indices;
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			indices.push_back(baseIndices[j] + 4 * i);
		}
	}
	AttributeDescriptor attributes[] = { {GL_FLOAT,3 },{GL_FLOAT,2},{GL_FLOAT,3} };
	return Mesh(vertices, indices, attributes, 3);
}
