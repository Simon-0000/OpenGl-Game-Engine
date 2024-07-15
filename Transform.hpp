#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Bindable.hpp"
class Transform : public Bindable{
public:
	Transform(const glm::vec3& position = {1.0f,1.0f,1.0f}, const glm::vec3& rotation = {0,0,0}, const glm::vec3& scale = {1.0f,1.0f,1.0f}, Transform* parent = nullptr);
	Transform(const Transform& other);
	Transform& operator=(const Transform& other);
	void setPosition(const glm::vec3& position);
	void setRotation(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale);
	void setParent(Transform* parent);

	const glm::mat4& getModelMatrix();
	Transform& getParent() const;
	const Transform& getPosition() const;
	const Transform& getRotation() const;
	const Transform& getScale() const;
	void resetUpdatingFlag();

	void localBind() override{}
	void localUnbind() override{}

private:
	void updateModelMatrix();
	glm::vec3 position_, rotation_, scale_;
	glm::mat4 localModelMatrix_,modelMatrix_;
	bool modelNeedsUpdating_;
protected:
	Transform* parent_;
};
