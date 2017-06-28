/*****************************************************************************/
/*!
\file   Component.h
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
#pragma once

#include "precompiled.h"

namespace VK
{
	class Entity;

	enum ComponentType
	{
		CMP_TRANSFORM,
		CMP_MESH
	};

	class Component
	{
	public:
		Component();
		virtual ~Component() {};

		virtual std::string GetName();
		virtual void SetName(std::string name);
		virtual Entity* GetOwner();
		virtual void SetOwner(Entity* owner);
		virtual void Action();
	private:
		Entity* m_owner;
		std::string m_name;
		std::map<std::string, void*> m_cmpDataList;
		ComponentType m_cmpType;

		void InitCmpDataList();

	};
}
