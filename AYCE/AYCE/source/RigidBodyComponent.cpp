/*****************************************************************************/
/*!
\file   RigidBodyComponent.cpp
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
#include "RigidBodyComponent.h"

namespace VK
{
	RigidBodyComponent::RigidBodyComponent(): m_fHP(0)
	{
		m_Mass = 1.0f;
		m_InvMass = 0.0f;
		m_Friction = 0.0f;
		m_Restitution = 0.0f;
		m_isKnockback = COL_OBJ::COL_OBJ_START;
		m_shape = SidBox;
		m_Dir = glm::vec3(0, 0, 0);
		m_Acceleration = glm::vec3(0, 0, 0);
		m_AccumulatedForce = glm::vec3(0, 0, 0);
		m_Velocity = glm::vec3(0, 0, 0);
		m_PrevPosition = glm::vec3(0, 0, 0);
		m_IsStatic = false;
		m_aabbMax = glm::vec3(0);
		m_aabbMin = glm::vec3(0);
		m_isActive = false;
		m_ID = DEFAULT;
	}

	RigidBodyComponent::~RigidBodyComponent()
	{

	}

	void RigidBodyComponent::AddForce(glm::vec3 force)
	{
		m_AccumulatedForce += force;
	}

	void RigidBodyComponent::SetVelocity(glm::vec3 v)
	{
		m_Velocity = v;
	}
	void RigidBodyComponent::SetDirection(glm::vec3 dir)
	{
		m_Dir = dir;
	}
	void RigidBodyComponent::SetMass(float mass)
	{
		m_Mass = mass;
	}
	void RigidBodyComponent::SetFriction(float friction)
	{
		m_Friction = friction;
	}
	void RigidBodyComponent::SetKnockback(COL_OBJ knocback)
	{
		m_isKnockback = knocback;
		if (!knocback)
			m_Velocity = glm::vec3(0.f);
	}
	void RigidBodyComponent::SetShape(enumShape shape)
	{
		m_shape = shape;
	}
	void RigidBodyComponent::SetAcceleration(glm::vec3 acc)
	{
		m_Acceleration = acc;
	}

	void RigidBodyComponent::SetAAbbMax(glm::vec3 max)
	{
		m_aabbMax = max;
	}

	void RigidBodyComponent::SetAAbbMin(glm::vec3 min)
	{
		m_aabbMin = min;
	}

	void RigidBodyComponent::SetID(COMPONENTTYPE ID)
	{
		m_ID = ID;
	}

	void RigidBodyComponent::SetIsActive(bool isActive)
	{
		m_isActive = isActive;
	}

	void RigidBodyComponent::SetHP(float hp)
	{
		m_fHP = hp;
	}


	glm::vec3 RigidBodyComponent::GetVelocity() const
	{
		return m_Velocity;
	}
	glm::vec3 RigidBodyComponent::GetDirection() const
	{
		return m_Dir;
	}
	float RigidBodyComponent::GetMass() const
	{
		return m_Mass;
	}
	float RigidBodyComponent::GetFriction() const
	{
		return m_Friction;
	}
	int RigidBodyComponent::GetKnockback() const
	{
		return m_isKnockback;
	}
	enumShape RigidBodyComponent::GetShape() const
	{
		return m_shape;
	}
	glm::vec3 RigidBodyComponent::GetAcceleration() const
	{
		return m_Acceleration;
	}
	glm::vec3 RigidBodyComponent::GetAAbbMax() const
	{
		return m_aabbMax;
	}
	glm::vec3 RigidBodyComponent::GetAAbbMin() const
	{
		return m_aabbMin;
	}
	int RigidBodyComponent::GetID() const
	{
		return m_ID;
	}
	bool RigidBodyComponent::GetIsActive() const
	{
		return m_isActive;
	}
	float RigidBodyComponent::GetHP() const
	{
		return m_fHP;
	}
}