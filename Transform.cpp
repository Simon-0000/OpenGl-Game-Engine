#include "Transform.hpp"

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, Transform* parent) :
	position_(position), rotation_(rotation), scale_(scale), localModelMatrix_(1.0f),modelMatrix_(1.0f),modelNeedsUpdating_(true), parent_(parent)
{
	modelNeedsUpdating_ = true;
}

Transform::Transform(const Transform& other)
{
	*this = other;
}

Transform& Transform::operator=(const Transform& other)//doesnt copy parent and parentModel
{
	position_ = other.position_;
	rotation_ = other.rotation_;
	scale_ = other.scale_;
	localModelMatrix_ = other.localModelMatrix_;
	parent_ = other.parent_;
	modelMatrix_ = other.modelMatrix_;
	modelNeedsUpdating_ = true;
	return *this;
}

void Transform::setPosition(const glm::vec3& position)
{
	modelNeedsUpdating_ = true;
	position_ = position;
}

void Transform::setRotation(const glm::vec3& rotation)
{
	modelNeedsUpdating_ = true;
	rotation_ = rotation;
}

void Transform::setScale(const glm::vec3& scale)
{
	modelNeedsUpdating_ = true;
	scale_ = scale;
}

void Transform::setParent(Transform* parent)
{
	if (parent_)
		parent_->unlinkChild(this);
	parent_ = parent;
	parent_->linkChild(this);
	modelNeedsUpdating_ = true;
}

const glm::mat4& Transform::getModelMatrix()
{
	updateModelMatrix();
	return localModelMatrix_;
}

void Transform::updateModelMatrix()
{
	bool updateOccured = parent_ && parent_->modelNeedsUpdating_;
	if (modelNeedsUpdating_) {
		localModelMatrix_ = glm::mat4(1.0f);
		localModelMatrix_ = glm::translate(localModelMatrix_, position_);
		//localModelMatrix_ = glm::rotate(localModelMatrix_, (float)rotation_.length(), rotation_);
		localModelMatrix_ = glm::scale(localModelMatrix_, scale_);
		updateOccured = true;
	}

	if(updateOccured)
	{
		if (parent_)
			modelMatrix_ = parent_->getModelMatrix() * localModelMatrix_;
		else
			modelMatrix_ = localModelMatrix_;

		modelNeedsUpdating_ = true;
	}

	if(hasNoRelative())
		resetUpdatingFlag();
}
void Transform::resetUpdatingFlag() {
	modelNeedsUpdating_ = false;
	if (parent_ && parent_->modelNeedsUpdating_)
		parent_->resetUpdatingFlag();
}