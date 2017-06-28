/*****************************************************************************/
/*!
\file   Component.cpp
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the source file for Credit state of Component
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "Component.h"
#include "Entity.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace VK
{
	
	Component::Component(): m_owner(nullptr), m_cmpType(), m_name()
	{
	}


	std::string Component::GetName()
	{
		return m_name;
	}
	void Component::SetName(std::string name)
	{
		m_name = name;
	}
	Entity* Component::GetOwner()
	{
		return m_owner;
	}
	void Component::SetOwner(Entity* owner)
	{
		m_owner = owner;
	}

	void Component::Action()
	{

	}

	void Component::InitCmpDataList()
	{
	}


}