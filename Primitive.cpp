#include "Primitive.hpp"


//template<typename T>
//Primitive::Primitive(T* vertices)//, size_t verticesSize, unsigned int* indices, size_t indicesSize, AttributeDescriptor* attributes, size_t attributesSize)
//{

//}

void Primitive::localBind() {
	glBindVertexArray(id_);
}