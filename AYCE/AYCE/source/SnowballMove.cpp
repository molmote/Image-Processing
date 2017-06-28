/*****************************************************************************/
/*!
\file   SnowballMove.cpp
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for SnowballMove features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "SnowballMove.h"

namespace VK
{

	SnowballMove::SnowballMove()
	{
		m_speed = 3.f;
		m_falling = false;
	}
	SnowballMove::~SnowballMove()
	{

	}

	float SnowballMove::GetSnowballSpeed(void) const
	{
		return m_speed;
	}
	void SnowballMove::SetSnowballSpeed(float speed)
	{
		m_speed = speed;
	}
	bool SnowballMove::GetFallState(void) const
	{
		return m_falling;
	}
	void SnowballMove::SetFallState(bool fall)
	{
		m_falling = fall;
	}

}