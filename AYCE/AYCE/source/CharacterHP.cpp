/*****************************************************************************/
/*!
\file   CharacterHP.cpp
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
#include "CharacterHP.h"

namespace VK
{

	CharacterHP::CharacterHP()
	{
		m_HP = 100;
	}
	CharacterHP::~CharacterHP()
	{

	}

	float CharacterHP::GetHP() const
	{
		return m_HP;
	}

	void CharacterHP::SetHP(float hp)
	{
		m_HP = hp;
	}
}