#pragma once

#include <iostream>
#include <memory>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>
#include <utility>


struct KeyActionInput {
	int key;
	int action;
	bool operator==(const KeyActionInput& other) const {
		return other.key == key && other.action == action;
	}
};

template<>
struct std::hash<KeyActionInput> {
	std::size_t operator()(const KeyActionInput& p) const {
		return std::hash<int>{}(p.key) ^ std::hash<int>{}(p.action);
	}
};

class Inputs {
public:
	static void init(GLFWwindow* window);
	static void addKeyCallback(const KeyActionInput& keyCallbackInput, std::function<void()> func);
	static void addContinuousKeyCallback(const KeyActionInput& keyCallbackInput, std::function<void()> func);

	static void removeContinuousKeyCallback(const KeyActionInput& keyCallbackInput);

	static void checkContinuousInputs();

private:
	static inline GLFWwindow* window_ = nullptr;
	static inline std::vector<std::pair<KeyActionInput, std::function<void()>>> continuousInputs;
	static inline std::unordered_map<KeyActionInput, std::function<void()>> inputs;

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};