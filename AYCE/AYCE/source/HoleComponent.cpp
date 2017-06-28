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
#include "HoleComponent.h"

namespace VK
{
	HoleComponent::HoleComponent()
	{
		m_xzSize = glm::vec2(0);
	}
	HoleComponent::~HoleComponent()
	{

	}


	void HoleComponent::SetHoleSize(glm::vec2 xz)
	{
		m_xzSize = xz;
	}
	glm::vec2 HoleComponent::GetHoleSize(void)
	{
		return m_xzSize;
	}
}