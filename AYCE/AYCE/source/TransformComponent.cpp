/*****************************************************************************/
/*!
\file   TransformComponent.cpp
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for TransformComponent features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "TransformComponent.h"

namespace VK
{

	TransformComponent::TransformComponent() :
		m_translation(0.0f),
		m_scale(1.0f),
		m_rotation(0.0f)
	{
	}


	TransformComponent::~TransformComponent()
	{
	}
	//void TransformComponent::Initialize(JasonValue& value)
	//{
	//	value.GetTransformPosition;
	//}
	void TransformComponent::setTranslation(float x, float y, float z) {
		m_translation.x = x;
		m_translation.y = y;
		m_translation.z = z;
		computeTransformation();
	}

	void TransformComponent::setTranslation(const glm::vec3& trans) {
		m_translation = trans;
		computeTransformation();
	}

	void TransformComponent::setRotation(float rotX, float rotY, float rotZ) {
		m_rotation.x = rotX;
		m_rotation.y = rotY;
		m_rotation.z = rotZ;
		computeTransformation();
	}

	void TransformComponent::setRotation(const glm::vec3& rot) {
		m_rotation = rot;
		computeTransformation();
	}

	void TransformComponent::setScale(float ss) {
		m_scale.x = ss;
		m_scale.y = ss;
		m_scale.z = ss;
		computeTransformation();

	}
	void TransformComponent::setScale(float sx, float sy, float sz) {
		m_scale.x = sx;
		m_scale.y = sy;
		m_scale.z = sz;
		computeTransformation();
	}

	void TransformComponent::setScale(const glm::vec3& ss)
	{
		m_scale.x = ss.x;
		m_scale.y = ss.y;
		m_scale.z = ss.z;
		computeTransformation();
	}

	glm::vec3& TransformComponent::getTranslation() {
		return m_translation;
	}

	glm::vec3& TransformComponent::getRotation() {
		return m_rotation;
	}

	glm::vec3& TransformComponent::getScale() {
		return m_scale;
	}

	void TransformComponent::computeTransformation() {
		glm::mat4 t = glm::translate(glm::mat4(), m_translation);
		glm::mat4 r = glm::eulerAngleYXZ(m_rotation.y, m_rotation.x, m_rotation.z);
		glm::mat4 s = glm::scale(glm::mat4(), m_scale);
		m_transformation = t*r*s;
	}

	glm::mat4& TransformComponent::getTransformation() {
		return m_transformation;
	}

	void TransformComponent::print() {
		for (int i = 0; i < 4; i++) {
			glm::vec4 column = m_transformation[i];
			for (int j = 0; j < 4; j++) {
				std::cout << column[j];
				std::cout << ',';
			}
			std::cout << '\n';
		}
	}


	void TransformComponent::Update()
	{

	}


}