#pragma once
#include <iostream>
#include <set>
#include "Camera.hpp"
#include "GameObject.hpp"

class Renderer {
public:
	struct Iterator;
	Renderer(Camera* camera = nullptr);
	void addToOpaqueBuffer(GameObject* obj);
	void addToTransparentBuffer(GameObject* obj);

	void renderBuffers();
	void clearBuffers();
	void sortBuffers();
	void sortTransparentBuffer();

	bool operator()(GameObject* obj1, GameObject* obj2);
	Iterator begin();
	Iterator end();
	Camera* referenceCamera;
	
	struct Iterator {
	public:
		Iterator(Renderer* source, unsigned int index) :
			src(source), index(index) {}
		GameObject& operator*() {
			auto opaqueSize = src->opaqueBuffer_.size();
			if (index < opaqueSize)
				return *src->opaqueBuffer_[index];
			else
				return *src->transparentBuffer_[index - opaqueSize];
		}
		Iterator& operator++() {
			++index;
			return *this;
		}
		bool operator==(const Iterator& other) const {
			return other.index == index && src == other.src;
		}
	private:
		Renderer* src;
		unsigned int index;
	};
private:
	std::vector<GameObject*> opaqueBuffer_, transparentBuffer_;//todo add sorting when needed + add iterator for this 
};