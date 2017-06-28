/*****************************************************************************/
/*!
\file   RigidBodyComponent.h
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for RigidBodyComponent features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include "precompiled.h"
#include "Component.h"

namespace VK
{
	enum enumShape
	{
		SidCircle,
		SidBox,
		SidNumberOfShapes
	};

	enum COL_OBJ
	{
		COL_OBJ_START = 0,
		COL_NONE = 0,
		COL_SMALL_SNOW,
		COL_BIG_SNOW,
		COL_OBJ_SIZE
	};

	enum COMPONENTTYPE {
		DEFAULT = 0,
		CHARACTER1,
		CHARACTER2,
		SNOW,
		SNOW1,
		SNOW2,
		BIG_SNOW,
		PARTICLE,
		PARTICLEWALK,
		PARTICLECOLLISON,
		PARTICLEWIN,
		PARTICLEFIREWORKS,
		PARTICLECREATESNOW
	};

	const glm::vec3 C_RIGHT = glm::vec3(0.f, (0.17f*9.f), 0.f);
	const glm::vec3 C_LEFT = glm::vec3(0.f, -(0.17f*9.f), 0.f);
	const glm::vec3 C_UP = glm::vec3(0.f, (0.17f*18.f), 0.f);
	const glm::vec3 C_DOWN = glm::vec3(0.f, 0.f, 0.f);

	const glm::vec3 S_RIGHT = glm::vec3(1.f, 0.f, 0.f);
	const glm::vec3 S_LEFT = glm::vec3(-1.f, 0.f, 0.f);
	const glm::vec3 S_UP = glm::vec3(0.f, 0.f, -1.f);
	const glm::vec3 S_DOWN = glm::vec3(0.f, 0.f, 1.f);


	class RigidBodyComponent : public Component
	{
	public:
		RigidBodyComponent();
		virtual ~RigidBodyComponent();


		void AddForce(glm::vec3 force);
		void SetVelocity(glm::vec3);
		void SetDirection(glm::vec3);
		void SetMass(float mass);
		void SetFriction(float friction);
		void SetKnockback(COL_OBJ knocback);
		void SetShape(enumShape shape);
		void SetAcceleration(glm::vec3 acc);
		void SetAAbbMax(glm::vec3 max);
		void SetAAbbMin(glm::vec3 min);
		void SetID(COMPONENTTYPE ID);
		void SetIsActive(bool isActive);
		void SetHP(float hp);

		glm::vec3 GetVelocity() const;
		glm::vec3 GetDirection() const;
		float GetMass() const;
		float GetFriction() const;
		int GetKnockback() const;
		enumShape GetShape() const;
		glm::vec3 GetAcceleration() const;
		glm::vec3 GetAAbbMax() const;
		glm::vec3 GetAAbbMin() const;
		int GetID() const;
		bool GetIsActive() const;
		float GetHP() const;
		
	private:
		glm::vec3 m_PrevPosition;
		glm::vec3 m_Velocity;
		glm::vec3 m_Acceleration;
		glm::vec3 m_Dir;
		glm::vec3 m_AccumulatedForce;

		bool m_IsStatic;
		COMPONENTTYPE m_ID;
		float m_Mass;
		float m_InvMass;
		float m_Restitution;
		float m_Friction;
		COL_OBJ m_isKnockback;
		enumShape m_shape;
		glm::vec3 m_aabbMax;
		glm::vec3 m_aabbMin;
		bool m_isActive;
		float m_fHP;
	};
}