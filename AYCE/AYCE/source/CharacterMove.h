/*****************************************************************************/
/*!
\file   CharacterMove.h
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
#pragma once
#include "Component.h"
#include "precompiled.h"

namespace VK
{
	enum COLLISION_FLAG
	{
		COL_LEFT = 1 << 0,
		COL_UP = 1 << 1,
		COL_RIGHT = 1 << 2,
		COL_DOWN = 1 << 3
	};
	class CharacterMove : public Component
	{
	public:
		CharacterMove();
		virtual ~CharacterMove();

		float& GetSpeed();
		void SetSpeed(float speed);
		void SetFallState(bool fall);
		bool GetFallState(void);
		void SetSnowball(Entity* eSnow);
		void ReSetSnowball();
		Entity* GetSnowball();
		void DeleteSnowball();

		/*void SetCollisionFlag(COLLISION_FLAG);
		bool IsMovable(COLLISION_FLAG);*/
		void SetCollidedObj(Entity* eObj);
		void ReSetCollidedObj();
		Entity* GetCollidedObj();
		void DeleteCollidedObj();
		

		void SetCollisionFlag(COLLISION_FLAG);
		bool IsMovable(COLLISION_FLAG) const;
		void ResetCollisionFlag();

	private:
		float m_speed;
		bool m_falling;

		unsigned m_col_flag;

		Entity* m_snowball;
		Entity* m_collidedObj;
	};
}