#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Bindable.hpp"
class Transform : public Bindable{
public:
	Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, Transform* parent);
	Transform(const Transform& other);
	Transform& operator=(const Transform& other);
	void setPosition(const glm::vec3& position);
	void setRotation(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale);
	const glm::mat4& getModelMatrix();
	Transform& getParent() const;
	const Transform& getPosition() const;
	const Transform& getRotation() const;
	const Transform& getScale() const;
	void resetUpdatingFlag();
private:
	void updateModelMatrix();
	glm::vec3 position_, rotation_, scale_;
	glm::mat4 localModelMatrix_,modelMatrix_;
	bool modelNeedsUpdating_;
protected:
	Transform* parent_;
};
