#include "Primitive.hpp"

void Primitive::localBind() {
	glBindVertexArray(id_);
}

void Primitive::localUnbind() {
	glBindVertexArray(0);
}

void Primitive::draw() {
	bind();//binding and unbinding might be unnecessary to be done here later on
	shader_->useThenSetMat4f("uModel", &getModelMatrix());
	glDrawElements(GL_TRIANGLES, getNumberOfIndices(), GL_UNSIGNED_INT, 0);
	unbind();
}

unsigned int Primitive::getNumberOfIndices() {
	return numberOfIndices_;
}


Primitive Primitive::cube(Shader* shader, const Transform& transInfo, float halfSideLength) {
	std::vector<BasicVertex> vertices = {
		{{-halfSideLength, -halfSideLength, -halfSideLength}, {0.0f,0.0f}, {0.0f,  0.0f, -1.0f}},
		{{ halfSideLength, -halfSideLength, -halfSideLength}, {1.0f,0.0f}, {0.0f,  0.0f, -1.0f}},
		{{ halfSideLength,  halfSideLength, -halfSideLength}, {1.0f,1.0f}, {0.0f,  0.0f, -1.0f}},
		{{-halfSideLength,  halfSideLength, -halfSideLength}, {0.0f,1.0f}, {0.0f,  0.0f, -1.0f}},
					   			 
		{{-halfSideLength, -halfSideLength,  halfSideLength}, {0.0f,0.0f},  {0.0f,  0.0f,  1.0f}},
		{{ halfSideLength, -halfSideLength,  halfSideLength}, {1.0f,0.0f},  {0.0f,  0.0f,  1.0f}},
		{{ halfSideLength,  halfSideLength,  halfSideLength}, {1.0f,1.0f},  {0.0f,  0.0f,  1.0f}},
		{{-halfSideLength,  halfSideLength,  halfSideLength}, {0.0f,1.0f},  {0.0f,  0.0f,  1.0f}},
					  			 
		{{-halfSideLength,  halfSideLength,  halfSideLength}, {1.0f,1.0f}, {-1.0f,  0.0f,  0.0f}},
		{{-halfSideLength,  halfSideLength, -halfSideLength}, {0.0f,1.0f}, {-1.0f,  0.0f,  0.0f}},
		{{-halfSideLength, -halfSideLength, -halfSideLength}, {0.0f,0.0f}, {-1.0f,  0.0f,  0.0f}},
		{{-halfSideLength, -halfSideLength,  halfSideLength}, {1.0f,0.0f}, {-1.0f,  0.0f,  0.0f}},
					  			 
		{{ halfSideLength,  halfSideLength,  halfSideLength}, {1.0f,1.0f}, {1.0f,  0.0f,  0.0f}},
		{{ halfSideLength,  halfSideLength, -halfSideLength}, {0.0f,1.0f}, {1.0f,  0.0f,  0.0f}},
		{{ halfSideLength, -halfSideLength, -halfSideLength}, {0.0f,0.0f}, {1.0f,  0.0f,  0.0f}},
		{{ halfSideLength, -halfSideLength,  halfSideLength}, {1.0f,0.0f}, {1.0f,  0.0f,  0.0f}},
					  			 
		{{-halfSideLength, -halfSideLength, -halfSideLength}, {0.0f,0.0f}, {0.0f, -1.0f,  0.0f}},
		{{ halfSideLength, -halfSideLength, -halfSideLength}, {1.0f,0.0f}, {0.0f, -1.0f,  0.0f}},
		{{ halfSideLength, -halfSideLength,  halfSideLength}, {1.0f,1.0f}, {0.0f, -1.0f,  0.0f}},
		{{-halfSideLength, -halfSideLength,  halfSideLength}, {0.0f,1.0f}, {0.0f, -1.0f,  0.0f}},
		   			   			 
		{{-halfSideLength,  halfSideLength, -halfSideLength}, {0.0f,1.0f}, {0.0f,  1.0f,  0.0f}},
		{{ halfSideLength,  halfSideLength, -halfSideLength}, {1.0f,1.0f}, {0.0f,  1.0f,  0.0f}},
		{{ halfSideLength,  halfSideLength,  halfSideLength}, {1.0f,0.0f}, {0.0f,  1.0f,  0.0f}},
		{{-halfSideLength,  halfSideLength,  halfSideLength}, {0.0f,0.0f}, {0.0f,  1.0f,  0.0f}},
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
	return Primitive(vertices, indices, attributes, 3, shader, transInfo);
}