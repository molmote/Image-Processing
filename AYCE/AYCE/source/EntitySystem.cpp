/*****************************************************************************/
/*!
\file   EntitySystem.cpp
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for EntitySystem features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "EntitySystem.h"
#include "Entity.h"
#include "Component.h"
#include "precompiled.h"
//#include "EntityMessage.h"
namespace VK
{

	EntitySystem::EntitySystem() : m_idStack (0)
	{
	}


	EntitySystem::~EntitySystem()
	{
		ClearEntity();
	}

	void EntitySystem::Init()
	{

	}


	void EntitySystem::Update(float dt)
	{
		//ComponentList a;
		//for (auto it : m_entityList)
		//{
		//	a = it->GetComponentList();

		//	for each(auto& it2 in a)
		//	{
		//		it2.second->Action();
		//	}
		//}
	}

	void EntitySystem::Shutdown()
	{

	}
	void EntitySystem::ProcessMessage(Message* message)
	{

	}

	void EntitySystem::AddEntity(Entity* entity)
	{
		static unsigned i = 0;
		START();
		ListType type = entity->GetListType();
			switch (type)
			{
			case E_DYNAMIC:
				m_dynamicList.push_back(entity);
				entity->SetID(GenerateID());
				break;
			case E_STATIC:
				m_staticList.push_back(entity);
				entity->SetID(GenerateID());
				break;
			case E_DEBUG:
				m_debugList.push_back(entity);
				entity->SetID(GenerateID());
				break;
			default:
				break;
			}
		LOG("Entity type is :", static_cast<unsigned>(type), entity->GetName(), entity->m_id, ++i);
	}
	void EntitySystem::DeleteEntity(Entity* entity)
	{
		if (entity->GetListType() == E_DYNAMIC)
		{
			auto it = std::find(m_dynamicList.begin(), m_dynamicList.end(), entity);
			entity->ClearComponent();
			if (it != m_dynamicList.end())
			{
				m_dynamicList.erase(it);
				delete entity;
			}
		}
		else if (entity->GetListType() == E_STATIC)
		{
			auto it = std::find(m_staticList.begin(), m_staticList.end(), entity);
			entity->ClearComponent();
			if (it != m_staticList.end())
			{
				m_staticList.erase(it);
				delete entity;
			}
		}
		else if (entity->GetListType() == E_DEBUG)
		{
			auto it = std::find(m_debugList.begin(), m_debugList.end(), entity);
			entity->ClearComponent();
			if (it != m_debugList.end())
			{
				m_debugList.erase(it);
				delete entity;
			}
		}
		
	}
	
	void EntitySystem::BroadcastEntityMessage(EMessage* message)
	{
		for (auto it : m_entityList)
		{
			it->SendEntityMessage(message);
		}

	}

	int EntitySystem::GenerateID()
	{
		return ++m_idStack;
	}
	void EntitySystem::ClearEntity()
	{

		//dynamic list delete
		for (auto it : m_dynamicList)
		{
			it->ClearComponent();
			delete it;
		}
		//static list delete
		for (auto it : m_staticList)
		{
			it->ClearComponent();
			delete it;
		}

		//debug list
		for (auto it : m_debugList)
		{

			it->ClearComponent();
			delete it;
		}

		m_dynamicList.clear();
		m_staticList.clear();
		m_debugList.clear();

		vector<Entity*>(m_dynamicList).swap(m_dynamicList);
		vector<Entity*>(m_staticList).swap(m_staticList);
		vector<Entity*>(m_debugList).swap(m_debugList);

		m_idStack = 0;
	}
	void EntitySystem::ClearDebugEntity()
	{
		for (auto it : m_debugList)
		{

			it->ClearComponent();

			delete it;
		}
		m_debugList.clear();
		m_debugList.resize(0);

		m_idStack = 0;

	}



	EntityListVector EntitySystem::GetDynamicEntityList() const
	{
		return m_dynamicList;
	}
	EntityListVector EntitySystem::GetStaticEntityList() const
	{
		return m_staticList;
	}
	EntityListVector EntitySystem::GetDebugEntityList() const
	{
		return m_debugList;
	}


}