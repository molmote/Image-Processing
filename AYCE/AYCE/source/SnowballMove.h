/*****************************************************************************/
/*!
\file   SnowballMove.h
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
#pragma once
#include "Component.h"
#include "precompiled.h"

namespace VK
{
	class SnowballMove : public Component
	{
	public:
		SnowballMove();
		virtual ~SnowballMove();

		float GetSnowballSpeed() const;
		void SetSnowballSpeed(float speed);

		bool GetFallState(void) const;
		void SetFallState(bool fall);

	private:
		float m_speed;
		bool m_falling;
	};
}