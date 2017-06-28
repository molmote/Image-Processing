/*****************************************************************************/
/*!
\file   CharacterMove.cpp
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for CharacterMove features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "CharacterMove.h"
#include "Application.h"
#include "EntitySystem.h"
#include "RigidBodyComponent.h"

namespace VK
{

	CharacterMove::CharacterMove()
	{
		m_speed = 0;
		m_col_flag = 0;
		m_falling = false;
		m_snowball = nullptr;
		m_collidedObj = nullptr;
	}

	CharacterMove::~CharacterMove()
	{

	}

	float& CharacterMove::GetSpeed()
	{
		return m_speed;
	}

	void CharacterMove::SetSpeed(float speed)
	{
		m_speed = speed;
	}

	void CharacterMove::SetCollisionFlag(COLLISION_FLAG flag)
	{
		m_col_flag |= flag;
	}

	bool CharacterMove::IsMovable(COLLISION_FLAG flag) const
	{
		return !(m_col_flag & flag);
	}

	void CharacterMove::ResetCollisionFlag()
	{
		m_col_flag = 0;
	}

	void CharacterMove::SetFallState(bool fall)
	{
		m_falling = fall;
	}
	bool CharacterMove::GetFallState(void)
	{
		return m_falling;
	}
	void CharacterMove::SetSnowball(Entity* eSnow)
	{
		m_snowball = eSnow;
	}
	Entity* CharacterMove::GetSnowball()
	{
		return m_snowball;
	}

	void CharacterMove::DeleteSnowball()
	{
		if (m_snowball)
		{
			Application::GetAppInstance()->getSystem<EntitySystem>()->DeleteEntity(m_snowball);
			m_snowball = nullptr;
		}
	}

	void CharacterMove::ReSetSnowball()
	{
		SetSnowball(nullptr);
	}

	void CharacterMove::SetCollidedObj(Entity* eObj)
	{
		if (nullptr == m_collidedObj)
			m_collidedObj = eObj;
		else if (m_collidedObj->FindComponent<RigidBodyComponent>()->GetID() <= eObj->FindComponent<RigidBodyComponent>()->GetID())
			m_collidedObj = eObj;
	}

	void CharacterMove::ReSetCollidedObj()
	{
		SetCollidedObj(nullptr);
	}

	Entity* CharacterMove::GetCollidedObj()
	{
		return m_collidedObj;
	}

	void CharacterMove::DeleteCollidedObj()
	{
		if (m_collidedObj)
		{
			m_collidedObj->SetAlive(false);
			//Application::GetAppInstance()->getSystem<EntitySystem>()->DeleteEntity(m_collidedObj);
			m_collidedObj = nullptr;
		}
		else
			cout << "_______________________________" << endl;
	}
}
