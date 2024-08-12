#pragma once
#include <iostream>
#include <set>
#include "Camera.hpp"
#include "GameObject.hpp"
class Renderer {
public:
	void addToBuffer(GameObject* obj);
	void renderFrame();
	void clearBuffer();
private:
	//std::set<>
};