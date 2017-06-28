/*****************************************************************************/
/*!
\file   Entity.h
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
#pragma once

#include "EntityMessage.h"
#include "Model.h"
#include <string>
#include <map>
#include <typeinfo>
#include<iostream>
#include "Component.h"
#include "MeshComponent.h"

namespace VK
{
	class Component;
	class Model;

	typedef  std::map<std::string, Component*> ComponentList;

	enum EntityType
	{
		E_PLAYER1,
		E_PLAYER2,
		E_SNOWBALL,
		E_SNOWBALL1,
		E_SNOWBALL2,
		E_MAP1,
		E_PARTICLE,
		E_UI,
		E_ITEM,
		E_HOLE,
		E_OBSTACLE_SNOWBALL

	};
	enum ItemType
	{
		E_ITEM_HP,
		E_ITEM_SPEED_UP,
		E_ITEM_SPEED_DOWN,
		E_ITEM_LAST
	};
	enum ListType
	{
		E_DYNAMIC,
		E_STATIC,
		E_DEBUG
	};
	
	class Entity
	{
	public:
		Entity();
		~Entity() {};

		void AddComponent(std::string name, Component* component);
		Component* GetComponent(std::string name);
		void RemoveComponent(std::string name);
		void ClearComponent();
		std::string GetName() const;
		void SetName(std::string name);
		void SetID(int id);
		ComponentList& GetComponentList();

		void SetListType(ListType type);
		void SetListType(std::string type);
		ListType GetListType() const;
		std::string TranslateListType(ListType type) const;
		ListType TranslateListType(std::string str);

		void SetEntityType(EntityType type);
		void SetEntityType(std::string type);
		EntityType GetEntityType() const;
		std::string TranslateEntityType(EntityType type) const;
		EntityType TranslateEntityType(std::string str) const;

		virtual void SendEntityMessage(EMessage* m) {};

		void SetAlive(bool flag);
		bool GetAlive() const;

		template<typename T>
		T* FindComponent()
		{
			for (auto &it : m_cmpList)
			{
				if (typeid(*it.second) == typeid(T))
				{
					return static_cast<T*>(it.second);
				}
			}
			return nullptr;
		}

		
		int m_id;

		
	private:
		std::string m_name;
		ComponentList m_cmpList;
		ListType m_listType;
		EntityType m_entityType;
	//	Model m_model;
		bool m_isAlive;
		
	};
}
