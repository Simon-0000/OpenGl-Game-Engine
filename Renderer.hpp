#pragma once
#include <iostream>
#include <set>
#include "Camera.hpp"
#include "GameObject.hpp"

class Renderer {
public:
	Renderer(Camera* camera = nullptr);
	void addToOpaqueBuffer(GameObject* obj);
	void addToTransparentBuffer(GameObject* obj);

	void renderBuffers();
	void clearBuffers();
	void sortBuffers();
	void sortTransparentBuffer();

	bool operator()(GameObject* obj1, GameObject* obj2);
	Camera* referenceCamera;
	struct objIterator {
	public:
		objIterator(std::vector<GameObject*>* source, unsigned int index) :
			src(source), index(index) {}
		GameObject& operator*() {
			return *(*src)[index];
		}
		objIterator operator++() {
			return objIterator{ src,index + 1 };
		}
		bool operator==(const objIterator& other) const{
			return other.index == index && src == other.src;
		}
	private:
		std::vector<GameObject*>* src;
		unsigned int index;
	};
private:
	std::vector<GameObject*> opaqueBuffer_, transparentBuffer_;//todo add sorting when needed + add iterator for this 
};