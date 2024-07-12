#include "Inputs.hpp"

void Inputs::init(GLFWwindow* window) {
	window_ = window;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, keyCallback);
}
void Inputs::addKeyCallback(const KeyActionInput& keyActionInput, std::function<void()> func) {
	inputs[keyActionInput] = func;
}
void Inputs::addContinuousKeyCallback(const KeyActionInput& keyActionInput, std::function<void()> func) {
	addKeyCallback({ keyActionInput.key ,GLFW_PRESS }, [=]() {continuousInputs.push_back({ keyActionInput,func });});
	addKeyCallback({ keyActionInput.key ,GLFW_RELEASE }, [=]() {removeContinuousKeyCallback(keyActionInput);});
}

void Inputs::removeContinuousKeyCallback(const KeyActionInput& keyActionInput) {
	auto it = std::find_if(continuousInputs.begin(), continuousInputs.end(), [keyActionInput](const auto& input) {return input.first == keyActionInput; });
	if (it != continuousInputs.end())
		continuousInputs.erase(it);
}

void Inputs::checkContinuousInputs() {
	for (const auto& input : continuousInputs) {
		if (glfwGetKey(window_, input.first.key) == input.first.action) {
			input.second();
		}
	}
}

void Inputs::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto it = inputs.find({ key,action });
	if (it != inputs.end())
		it->second();
}