#include "Transform.hpp"

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, Transform* parent) :
	position_(position), scale_(scale), rotation2_(rotation), localModelMatrix_(1.0f), modelMatrix_(1.0f), localModelNeedsUpdating_(true), childModelNeedsUpdating_(true), parent_(parent), childCount_(0)
{
	if (parent)
		childId_ = ++parent->childCount_;
	localModelNeedsUpdating_ = true;
}

Transform::Transform(const Transform& other)
{
	*this = other;
}

Transform& Transform::operator=(const Transform& other)//doesnt copy parent and parentModel
{
	position_ = other.position_;
	rotation2_ = other.rotation2_;
	scale_ = other.scale_;
	localModelMatrix_ = other.localModelMatrix_;
	modelMatrix_ = other.modelMatrix_;
	parent_ = other.parent_;
	localModelNeedsUpdating_ = true;
	childModelNeedsUpdating_ = true;
	childId_ = parent_ ? childId_ = ++parent_->childCount_ : 0;
	return *this;
}

void Transform::setPosition(const glm::vec3& position)
{
	localModelNeedsUpdating_ = true;
	position_ = position;
}
const glm::vec3& Transform::getPosition() const
{
	return position_;
}
const glm::quat& Transform::getRotation() const
{
	return rotation2_;
}

const glm::vec3& Transform::getRight() const
{
	return glm::vec3(modelMatrix_[0][0], modelMatrix_[0][1], modelMatrix_[0][2]);
}
const glm::vec3& Transform::getUp() const
{
	return glm::vec3(modelMatrix_[1][0], modelMatrix_[1][1], modelMatrix_[1][2]);
}
const glm::vec3& Transform::getForward() const
{
	return glm::vec3(modelMatrix_[2][0], modelMatrix_[2][1], modelMatrix_[2][2]);
}
const glm::vec3& Transform::getGlobalPosition() const
{
	return glm::vec3(modelMatrix_[3][0], modelMatrix_[3][1], modelMatrix_[3][2]);
}
void Transform::translate(const glm::vec3& translation)
{
	localModelNeedsUpdating_ = true;
	position_ += translation;
}

void Transform::setRotation(const glm::vec3& rotation)
{
	localModelNeedsUpdating_ = true;
	rotation2_ = glm::quat(rotation);
}
void Transform::setRotation(const glm::quat& rotation)
{
	localModelNeedsUpdating_ = true;
	rotation2_ = rotation;
}

void Transform::rotate(const glm::vec3& rotation) {
	localModelNeedsUpdating_ = true;
	rotation2_ *= glm::quat(rotation);
}

void Transform::setScale(const glm::vec3& scale)
{
	localModelNeedsUpdating_ = true;
	scale_ = scale;
}

void Transform::setParent(Transform* parent)
{
	if (parent_ = parent)
		childId_ = ++parent->childCount_;
	else
		childId_ = 0;
	childModelNeedsUpdating_ = true;
}

const glm::mat4& Transform::getUpdatedModelMatrix()
{
	tryUpdateModelMatrix();
	return getModelMatrix();
}

const glm::mat4& Transform::getModelMatrix() const
{
	return modelMatrix_;
}

bool Transform::tryUpdateModelMatrix()
{
	bool updateOccured = parent_ && parent_->childModelNeedsUpdating_;
	if (localModelNeedsUpdating_) {
		localModelMatrix_ = glm::mat4(1.0f);
		localModelMatrix_ = glm::translate(localModelMatrix_, position_);
		localModelMatrix_ *= glm::mat4_cast(rotation2_);//TODO use a bool for each transformation because individual transformations eat a lot of resources which could 
		localModelMatrix_ = glm::scale(localModelMatrix_, scale_);
		localModelNeedsUpdating_ = false;
		updateOccured = true;
	}

	if (updateOccured)
	{
		if (parent_)
			modelMatrix_ = parent_->getUpdatedModelMatrix() * localModelMatrix_;
		else
			modelMatrix_ = localModelMatrix_;

		childModelNeedsUpdating_ = true;

		if (parent_ && childId_ == parent_->childCount_)
			parent_->childModelNeedsUpdating_ = false;
		else if (childCount_ == 0)
			childModelNeedsUpdating_ = false;
	}
	return updateOccured;
}


