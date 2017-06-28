/*****************************************************************************/
/*!
\file   TransformComponent.h
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
#pragma once
#include "precompiled.h"

#include "Component.h"


namespace VK
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent();
		virtual ~TransformComponent();

		glm::mat4& getTransformation();
		glm::vec3& getTranslation();
		glm::vec3& getScale();
		glm::vec3& getRotation();
		void setTranslation(float x, float y, float z);
		void setTranslation(const glm::vec3& trans);
		void setRotation(float rotX, float rotY, float floatZ);
		void setRotation(const glm::vec3& rot);
		void setScale(float ss);
		void setScale(float sx, float sy, float sz);
		void setScale(const glm::vec3& scale);
		void print();

		//void Initialize(JasonValue& value);

		virtual void Update();
	private:
		glm::vec3 m_translation;
		glm::vec3 m_scale;
		glm::vec3 m_rotation;
		glm::mat4 m_transformation;
		void computeTransformation();

	};

}
