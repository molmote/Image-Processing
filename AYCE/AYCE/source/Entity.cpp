/*****************************************************************************/
/*!
\file   Entity.cpp
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Entity features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "Entity.h"
#include "MeshComponent.h"
namespace VK
{
	Entity::Entity(): m_id(0), m_listType(E_DEBUG), m_isAlive(true)//, m_model("sphere.obj")
	{
	}

	void Entity::AddComponent(std::string name, Component* component)
	{
		if (name == "")
			return;

		m_cmpList.emplace(name.c_str(), component);

		//SetName(name);
		//set owner
		
		//set comp name
	}
	Component* Entity::GetComponent(std::string id)
	{
		if (m_cmpList.find(id) == m_cmpList.end())
			return nullptr;
		else
			return m_cmpList.find(id)->second;

	}
	void Entity::RemoveComponent(std::string id)
	{
		m_cmpList.erase(id);
	}
	void Entity::ClearComponent()
	{
		if (m_cmpList.empty())
			return;
		else
		{
			for (auto &it : m_cmpList)
			{
				//std::cout << it.second->GetOwner()->GetName() << std::endl;
				//if (typeid(it.second) == typeid(MeshComponent*)){}
				//	//static_cast<MeshComponent*>(it.second)->~MeshComponent();
				//else
				
						delete it.second;
			}
		}

		m_cmpList.clear();
		
	}
	std::string Entity::GetName() const
	{
		return m_name;
	}

	void Entity::SetID(int id)
	{
		m_id = id;
	}

	ComponentList& Entity::GetComponentList()
	{
		return m_cmpList;
	}
	void Entity::SetName(std::string name)
	{
		m_name = name;
	}
	void Entity::SetEntityType(EntityType type)
	{
		m_entityType = type;
	}

	void Entity::SetEntityType(std::string type)
	{
		m_entityType = TranslateEntityType(type);
	}
	EntityType Entity::GetEntityType() const
	{
		return m_entityType;
	}


	std::string Entity::TranslateEntityType(EntityType type) const
	{
		switch (type)
		{
		case E_PLAYER1:
			return "E_PLAYER1";
			break;

		case E_PLAYER2:
			return "E_PLAYER2";
			break;

		case E_SNOWBALL:
			return "E_SNOWBALL";
			break;

		case E_SNOWBALL1:
			return "E_SNOWBALL1";
			break;

		case E_SNOWBALL2:
			return "E_SNOWBALL2";
			break;

		case E_MAP1:
			return "E_MAP1";
			break;

		case E_UI:
			return "E_UI";
			break;

		case E_PARTICLE:
			return "E_PARTICLE";
			break;

		case E_ITEM:
			return "E_ITEM";
			break;
		case E_HOLE:
			return "E_HOLE";
			break;
		default:
			return "None";
		}
	}
	EntityType Entity::TranslateEntityType(std::string str) const
	{
		if (str == "E_PLAYER1")
			return E_PLAYER1;
		else if (str == "E_PLAYER2")
			return E_PLAYER2;
		else if (str == "E_SNOWBALL")
			return E_SNOWBALL;
		else if (str == "E_SNOWBALL1")
			return E_SNOWBALL1;
		else if (str == "E_SNOWBALL2")
			return E_SNOWBALL2;
		else if (str == "E_MAP1")
			return E_MAP1;
		else if (str == "E_PARTICLE")
			return E_PARTICLE;
		else if (str == "E_ITEM")
			return E_ITEM;
		else if (str == "E_HOLE")
			return E_HOLE;
		else
			return E_UI;
	}


	void Entity::SetListType(ListType type)
	{
		m_listType = type;
	}
	void Entity::SetListType(std::string type)
	{
		m_listType = TranslateListType(type);
	}
	ListType Entity::GetListType() const
	{
		return m_listType;
	}


	std::string Entity::TranslateListType(ListType type) const
	{
		switch (type)
		{
		case E_DYNAMIC:
			return "E_DYNAMIC";
		case E_STATIC:
			return "E_STATIC";
		case E_DEBUG:
			return "E_DEBUG";
		default:
			return "None";
		}
	}
	ListType Entity::TranslateListType(std::string str)
	{
		if (str == "E_DYNAMIC")
			return E_DYNAMIC;
		else if (str == "E_STATIC")
			return E_STATIC;
		else
			return E_DEBUG;
	}

	void Entity::SetAlive(bool flag)
	{
		m_isAlive = flag;
	}
	bool Entity::GetAlive() const
	{
		return m_isAlive;
	}
}