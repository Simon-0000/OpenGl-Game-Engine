#include "Transform.hpp"

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, Transform* parent) :
	position_(position), rotation_(rotation), scale_(scale), localModelMatrix_(1.0f),modelMatrix_(1.0f),modelNeedsUpdating_(true), parent_(parent), childCount_(0)
{
	if (parent)
		childId_ = ++parent->childCount_;
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
	modelMatrix_ = other.modelMatrix_;
	parent_ = other.parent_;
	modelNeedsUpdating_ = true;
	childId_ = parent_ ? childId_ = ++parent_->childCount_ : 0;
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
	if (parent_ = parent)
		childId_ = ++parent->childCount_;
	else
		childId_ = 0;
	modelNeedsUpdating_ = true;
}

const glm::mat4& Transform::getUpdatedModelMatrix()
{
	updateModelMatrix();
	return modelMatrix_;
}

void Transform::updateModelMatrix()
{
	bool updateOccured = parent_ && parent_->modelNeedsUpdating_;
	if (modelNeedsUpdating_) {
		localModelMatrix_ = glm::mat4(1.0f);
		localModelMatrix_ = glm::translate(localModelMatrix_, position_);
		if(glm::length(rotation_) > ZERO_LENGTH_EPSILON)//prevent zero length rotation (nan)
			localModelMatrix_ = glm::rotate(localModelMatrix_, glm::length(rotation_), glm::normalize(rotation_));
		localModelMatrix_ = glm::scale(localModelMatrix_, scale_);
		updateOccured = true;
	}

	if(updateOccured)
	{
		if (parent_)
			modelMatrix_ = parent_->getUpdatedModelMatrix() * localModelMatrix_;
		else
			modelMatrix_ = localModelMatrix_;

		modelNeedsUpdating_ = true;
	}

	if (parent_ && childId_ == parent_->childCount_)
		parent_->modelNeedsUpdating_ = false;
	else if (childCount_ == 0)
		modelNeedsUpdating_ = false;


}
