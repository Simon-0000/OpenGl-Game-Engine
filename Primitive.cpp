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


Primitive Primitive::cube(float halfSideLength) {
	//init 
	std::vector<BasicVertex> vertices = {
		{{-halfSideLength, -halfSideLength, -halfSideLength}, {0.0f,0.0f}, {0.0f,  0.0f, -1.0f}},
		{{ halfSideLength, -halfSideLength, -halfSideLength}, {1.0f,0.0f}, {0.0f,  0.0f, -1.0f}},
		{{ halfSideLength,  halfSideLength, -halfSideLength}, {1.0f,1.0f}, {0.0f,  0.0f, -1.0f}},
		{{-halfSideLength,  halfSideLength, -halfSideLength}, {1.0f,0.0f}, {0.0f,  0.0f, -1.0f}},
					   			 
		{{-halfSideLength, -halfSideLength,  halfSideLength}, {0.0f,0.0f},  {0.0f,  0.0f,  1.0f}},
		{{ halfSideLength, -halfSideLength,  halfSideLength}, {1.0f,0.0f},  {0.0f,  0.0f,  1.0f}},
		{{ halfSideLength,  halfSideLength,  halfSideLength}, {1.0f,1.0f},  {0.0f,  0.0f,  1.0f}},
		{{-halfSideLength,  halfSideLength,  halfSideLength}, {1.0f,0.0f},  {0.0f,  0.0f,  1.0f}},
					  			 
		{{-halfSideLength,  halfSideLength,  halfSideLength}, {0.0f,0.0f}, {-1.0f,  0.0f,  0.0f}},
		{{-halfSideLength,  halfSideLength, -halfSideLength}, {1.0f,0.0f}, {-1.0f,  0.0f,  0.0f}},
		{{-halfSideLength, -halfSideLength, -halfSideLength}, {1.0f,1.0f}, {-1.0f,  0.0f,  0.0f}},
		{{-halfSideLength, -halfSideLength,  halfSideLength}, {1.0f,0.0f}, {-1.0f,  0.0f,  0.0f}},
					  			 
		{{ halfSideLength,  halfSideLength,  halfSideLength}, {0.0f,0.0f}, {1.0f,  0.0f,  0.0f}},
		{{ halfSideLength,  halfSideLength, -halfSideLength}, {1.0f,0.0f}, {1.0f,  0.0f,  0.0f}},
		{{ halfSideLength, -halfSideLength, -halfSideLength}, {1.0f,1.0f}, {1.0f,  0.0f,  0.0f}},
		{{ halfSideLength, -halfSideLength,  halfSideLength}, {1.0f,0.0f}, {1.0f,  0.0f,  0.0f}},
					  			 
		{{-halfSideLength, -halfSideLength, -halfSideLength}, {0.0f,0.0f}, {0.0f, -1.0f,  0.0f}},
		{{ halfSideLength, -halfSideLength, -halfSideLength}, {1.0f,0.0f}, {0.0f, -1.0f,  0.0f}},
		{{ halfSideLength, -halfSideLength,  halfSideLength}, {1.0f,1.0f}, {0.0f, -1.0f,  0.0f}},
		{{-halfSideLength, -halfSideLength,  halfSideLength}, {1.0f,0.0f}, {0.0f, -1.0f,  0.0f}},
		   			   			 
		{{-halfSideLength,  halfSideLength, -halfSideLength}, {0.0f,0.0f}, {0.0f,  1.0f,  0.0f}},
		{{ halfSideLength,  halfSideLength, -halfSideLength}, {1.0f,0.0f}, {0.0f,  1.0f,  0.0f}},
		{{ halfSideLength,  halfSideLength,  halfSideLength}, {1.0f,1.0f}, {0.0f,  1.0f,  0.0f}},
		{{-halfSideLength,  halfSideLength,  halfSideLength}, {1.0f,0.0f}, {0.0f,  1.0f,  0.0f}},
	};

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
	AttributeDescriptor attributes[] = { {GL_FLOAT,3 },{GL_FLOAT,2},{GL_FLOAT,3} };
	return Primitive(vertices, indices, attributes, 3);
}