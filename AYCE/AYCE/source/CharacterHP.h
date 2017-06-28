/*****************************************************************************/
/*!
\file   CharacterHP.h
\author Hyeongjin Park
\par    email: park.hyungjin\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for CharacterHP features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include "Component.h"
#include "precompiled.h"

namespace VK
{
	class CharacterHP : public Component
	{
	public:
		CharacterHP();
		virtual ~CharacterHP();

		float GetHP() const;
		void SetHP(float hp);

	private:
		float m_HP;
	};
}