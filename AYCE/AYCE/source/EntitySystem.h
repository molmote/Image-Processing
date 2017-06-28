/*****************************************************************************/
/*!
\file   EntitySystem.h
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
#pragma once

#include "System.h"
#include "Entity.h"
#include <vector>
namespace VK
{
	class EMessage;

	typedef  std::vector<Entity*> EntityListVector;

	class EntitySystem : public System
	{
	public:
		EntitySystem();
		virtual ~EntitySystem();

		void Init() override;
		void Update(float dt) override;
		void Shutdown() override;
		void ProcessMessage(Message* message) override;

		
		void DeleteEntity(Entity* entity);
		void ClearEntity();
		void ClearDebugEntity();

		void BroadcastEntityMessage(EMessage* message);
		
		//template<typename T>
		//T* CreateEntity(std::string object_name)
		//{
		//	Entity* nEntity = new T;
		//	nEntity->SetID(GenerateID());
		//	nEntity->SetName(object_name);
		//	AddEntity(nEntity);

		//	return dynamic_cast<T*>(*nEntity);
		//}
		void AddEntity(Entity* entity);

		EntityListVector GetDynamicEntityList() const;
		EntityListVector GetStaticEntityList() const;
		EntityListVector GetDebugEntityList() const;

		
	private:
		EntityListVector m_entityList;
		EntityListVector m_dynamicList;
		EntityListVector m_staticList;
		EntityListVector m_debugList;

		int m_idStack;
		int GenerateID();

	};
}

