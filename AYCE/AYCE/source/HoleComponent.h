/*****************************************************************************/
/*!
\file   HoleComponent.cpp
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the source file for HoleComponent
Copyright 2017, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include "Component.h"
#include "precompiled.h"


namespace VK
{
	class HoleComponent : public Component
	{
	public:
		HoleComponent();
		virtual ~HoleComponent();
		void SetHoleSize(glm::vec2 xz);
		glm::vec2 GetHoleSize(void);
	private:
		glm::vec2 m_xzSize;
	};
}