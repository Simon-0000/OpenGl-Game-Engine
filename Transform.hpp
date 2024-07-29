#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Transform{
public:
	Transform(const glm::vec3& position = {1.0f,1.0f,1.0f}, const glm::vec3& rotation = {0,0,0}, const glm::vec3& scale = {1.0f,1.0f,1.0f}, Transform* parent = nullptr);
	Transform(const Transform& other);
	Transform& operator=(const Transform& other);
	void setPosition(const glm::vec3& position);
	void translate(const glm::vec3& translation);

	void setRotation(const glm::vec3& rotation);
	void rotate(const glm::vec3& rotation);

	void setScale(const glm::vec3& scale);
	void setParent(Transform* parent);

	const glm::mat4& getUpdatedModelMatrix();
	const glm::mat4& getModelMatrix() const;

	Transform* getParent() const;
	const glm::vec3& getPosition() const;
	const glm::vec3& getRotation() const;
	const glm::vec3& getScale() const;

	const glm::vec3& getRight() const;
	const glm::vec3& getUp() const;
	const glm::vec3& getForward() const;
	const glm::vec3& getGlobalPosition() const;


protected:
	virtual bool tryUpdateModelMatrix();
private:
	glm::vec3 position_, rotation_, scale_;
	glm::mat4 localModelMatrix_,modelMatrix_;
	bool localModelNeedsUpdating_, childModelNeedsUpdating_;
	static constexpr float ZERO_LENGTH_EPSILON = 0.0000000000000001f;
protected:
	Transform* parent_;
	int childCount_ = 0, childId_ = 0;//TODO add logic to refresh matrix if a model is drawn before its parent and its grandparents by propagating flag through grandchildrens

};
