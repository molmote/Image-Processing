/*****************************************************************************/
/*!
\file   Factory.cpp
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Factory features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include "Factory.h"
#include "Entity.h"
#include "EntitySystem.h"
#include "AudioSystem.h"
#include "Application.h"
#include "Component.h"
#include "GameLogic.h"
#include "Geometry.h"

//all the component header will be here
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "RigidBodyComponent.h"
#include "CharacterMove.h"
#include "CharacterHP.h"
#include "ItemUsedComponent.h"
#include "HoleComponent.h"
#include "SnowballMove.h"


namespace Factory
{

	std::string ReadFromFile(std::string filename)
	{
		std::ifstream file(filename.c_str(), std::ifstream::binary);

		if (!file)
			return nullptr;

		Json::Value root;
		file >> root;

		Json::StyledWriter writer;
		std::string result = writer.write(root);

		return result;
	}


	void CreateLevel(std::string filename)
	{
		VK::Application* app = VK::Application::GetAppInstance();
		VK::EntitySystem* m_pES = app->getSystem<VK::EntitySystem>();
		//set file location
		std::string level = ReadFromFile("Object\\" + filename);

		Json::Value root;
		Json::Reader reader;
		bool parsing = reader.parse(level, root);

		if (!parsing)
			return;
		//entitypart
		Json::Value entityPart = root["Entity"];
		std::vector<std::string> j_nameList = entityPart.getMemberNames();

		for (auto it : j_nameList)
		{
			VK::Entity* nEntity = CreateEntity(entityPart, it);
			nEntity->SetName(it.c_str());
			nEntity->SetEntityType(entityPart[it.c_str()]["Entity Type"].asString());
			nEntity->SetListType(entityPart[it.c_str()]["List Type"].asString());
			//add into entitysystem
			m_pES->AddEntity(nEntity);
		}


	}
	VK::Entity* CreateEntity(Json::Value entityPart, std::string e_name)
	{
		VK::Entity* entity = new VK::Entity;
		//entity type should go here
		//entityPart["type"]

		//

		//componentpart
		Json::Value cmpPart = entityPart[e_name.c_str()]["Component"];

		std::vector<std::string> j_cmpTypeList = cmpPart.getMemberNames();

		for (auto it : j_cmpTypeList)
		{
			VK::Component* nCmp = CreateComponent(cmpPart[it], it);
			nCmp->SetOwner(entity);
			//add component
			entity->AddComponent(it, nCmp);
			//VK::ComponentList &list = entity->GetComponentList();
			//list.emplace(it.c_str(), nCmp);
		}

		return entity;
	}

	//VK::Component* Factory::CreateComponent(const std::string& compType)
	//{
	//	if (componentType == "Transform")
	//		return new TransformComponent;
	//}

	VK::Component* CreateComponent(Json::Value componentPart, std::string componentType)
	{
		//Component* component = CreateComponent(componentType);
		//component->Initialize(componentPart);
		if (componentType == "Transform")
		{
			VK::TransformComponent* transform = new VK::TransformComponent;
			glm::vec3 position;
			position.x = componentPart["Position"][0].asFloat();
			position.y = componentPart["Position"][1].asFloat();
			position.z = componentPart["Position"][2].asFloat();

			transform->setTranslation(position);

			glm::vec3 scale;
			scale.x = componentPart["Scale"][0].asFloat();
			scale.y = componentPart["Scale"][1].asFloat();
			scale.z = componentPart["Scale"][2].asFloat();

			transform->setScale(scale);

			glm::vec3 rot;
			rot.x = componentPart["Rotation"][0].asFloat();
			rot.y = componentPart["Rotation"][1].asFloat();
			rot.z = componentPart["Rotation"][2].asFloat();
			transform->setRotation(rot);

			return transform;
		}
		else if (componentType == "MeshComponent")
		{
			VK::MeshComponent* mesh = new VK::MeshComponent;
			mesh->setMesh(componentPart["Mesh"].asString());

			return mesh;
		}
		else if (componentType == "RigidBody")
		{
			VK::RigidBodyComponent* rigidbody = new VK::RigidBodyComponent;

			glm::vec3 vel;
			vel.x = componentPart["Velocity"][0].asFloat();
			vel.y = componentPart["Velocity"][1].asFloat();
			vel.z = componentPart["Velocity"][2].asFloat();
			rigidbody->SetVelocity(vel);

			glm::vec3 acc;
			acc.x = componentPart["Acceleration"][0].asFloat();
			acc.y = componentPart["Acceleration"][1].asFloat();
			acc.z = componentPart["Acceleration"][2].asFloat();
			rigidbody->SetAcceleration(acc);

			glm::vec3 dir;
			dir.x = componentPart["Direction"][0].asFloat();
			dir.y = componentPart["Direction"][1].asFloat();
			dir.z = componentPart["Direction"][2].asFloat();
			rigidbody->SetDirection(dir);

			glm::vec3 max;
			max.x = componentPart["AabbMax"][0].asFloat();
			max.y = componentPart["AabbMax"][1].asFloat();
			max.z = componentPart["AabbMax"][2].asFloat();
			rigidbody->SetAAbbMax(max);

			glm::vec3 min;
			min.x = componentPart["AabbMin"][0].asFloat();
			min.y = componentPart["AabbMin"][1].asFloat();
			min.z = componentPart["AabbMin"][2].asFloat();
			rigidbody->SetAAbbMin(min);

			rigidbody->SetMass(componentPart["Mass"].asFloat());
			rigidbody->SetID(static_cast<VK::COMPONENTTYPE>(componentPart["ID"].asInt()));
			rigidbody->SetHP(componentPart["HP"].asFloat());
			rigidbody->SetFriction(componentPart["Friction"].asFloat());
			rigidbody->SetKnockback(static_cast<VK::COL_OBJ>(componentPart["Knockback"].asInt()));
			rigidbody->SetIsActive(componentPart["IsActive"].asBool());



			return rigidbody;
		}
		else if (componentType == "CharacterMove")
		{
			VK::CharacterMove* move = new VK::CharacterMove;
			move->SetSpeed(componentPart["Speed"].asFloat());
			return move;
		}
		else if (componentType == "CharacterHP")
		{
			VK::CharacterHP* hp = new VK::CharacterHP;
			hp->SetHP(componentPart["m_HP"].asFloat());
			return hp;
		}
		else if (componentType == "HoleComponent")
		{
			VK::HoleComponent *phole = new VK::HoleComponent;
			phole->SetHoleSize(glm::vec2(componentPart["HoleSize"][0].asFloat(), componentPart["HoleSize"][1].asFloat()));
			return phole;
		}
		//more component type will be here
		else
		{
			return nullptr;
		}


	}


	void LevelSave(std::string filename)
	{
		VK::Application* app = VK::Application::GetAppInstance();
		VK::EntitySystem* m_pES = app->getSystem<VK::EntitySystem>();

		Json::Value j_Level;

		VK::EntityListVector e_dynamiclist = m_pES->GetDynamicEntityList();
		if (!e_dynamiclist.empty())
		{
			j_Level["Entity"];

			for (auto i : e_dynamiclist)
			{
				//put entity name
				j_Level["Entity"][i->GetName().c_str()];
				//SaveEntity(j_Level["Entity"][i->GetName().c_str()], i);
				j_Level["Entity"][i->GetName().c_str()]["List Type"] = "E_DYNAMIC";
				j_Level["Entity"][i->GetName().c_str()]["Entity Type"] = i->TranslateEntityType(i->GetEntityType());


				SaveComponent(j_Level["Entity"][i->GetName().c_str()]["Component"], i);

			}
		}

		VK::EntityListVector e_staticlist = m_pES->GetStaticEntityList();
		if (!e_staticlist.empty())
		{
			j_Level["Entity"];

			for (auto i : e_staticlist)
			{
				//put entity name
				j_Level["Entity"][i->GetName().c_str()];
				//SaveEntity(j_Level["Entity"][i->GetName().c_str()], i);
				j_Level["Entity"][i->GetName().c_str()]["List Type"] = "E_STATIC";
				j_Level["Entity"][i->GetName().c_str()]["Entity Type"] = i->TranslateEntityType(i->GetEntityType());

				SaveComponent(j_Level["Entity"][i->GetName().c_str()]["Component"], i);

			}
		}

		VK::EntityListVector e_debuglist = m_pES->GetDebugEntityList();
		if (!e_debuglist.empty())
		{
			j_Level["Entity"];

			for (auto i : e_debuglist)
			{
				//put entity name
				j_Level["Entity"][i->GetName().c_str()];
				//SaveEntity(j_Level["Entity"][i->GetName().c_str()], i);
				j_Level["Entity"][i->GetName().c_str()]["List Type"] = "E_DEBUG";
				j_Level["Entity"][i->GetName().c_str()]["Entity Type"] = i->TranslateEntityType(i->GetEntityType());

				SaveComponent(j_Level["Entity"][i->GetName().c_str()]["Component"], i);

			}
		}
		std::string file = "Object\\" + filename;

		WriteToFile(file.c_str(), j_Level);
	}

	void SaveComponent(Json::Value& root, VK::Entity* entity)
	{
		VK::TransformComponent* transform = entity->FindComponent<VK::TransformComponent>();
		VK::MeshComponent* mesh = entity->FindComponent<VK::MeshComponent>();
		VK::RigidBodyComponent* rigidbody = entity->FindComponent<VK::RigidBodyComponent>();
		VK::CharacterMove* cmove = entity->FindComponent<VK::CharacterMove>();
		VK::CharacterHP* hp = entity->FindComponent<VK::CharacterHP>();
		VK::HoleComponent* phole = entity->FindComponent<VK::HoleComponent>();
		//std::cout << typeid(entity->FindComponent<VK::TransformComponent>()).name();

		if (transform)
		{
			Json::Value pos(Json::arrayValue);
			pos.append(Json::Value(transform->getTranslation().x));
			pos.append(Json::Value(transform->getTranslation().y));
			pos.append(Json::Value(transform->getTranslation().z));
			Json::Value rot(Json::arrayValue);
			rot.append(Json::Value(transform->getRotation().x));
			rot.append(Json::Value(transform->getRotation().y));
			rot.append(Json::Value(transform->getRotation().z));
			Json::Value scale(Json::arrayValue);
			scale.append(Json::Value(transform->getScale().x));
			scale.append(Json::Value(transform->getScale().y));
			scale.append(Json::Value(transform->getScale().z));
			root["Transform"]["Position"] = pos;
			root["Transform"]["Rotation"] = rot;
			root["Transform"]["Scale"] = scale;

		}
		if (mesh)
		{
			root["MeshComponent"]["Mesh"] = mesh->getMeshFile();
		}
		if (rigidbody)
		{
			Json::Value vel(Json::arrayValue);
			vel.append(Json::Value(rigidbody->GetVelocity().x));
			vel.append(Json::Value(rigidbody->GetVelocity().y));
			vel.append(Json::Value(rigidbody->GetVelocity().z));
			Json::Value dir(Json::arrayValue);
			dir.append(Json::Value(rigidbody->GetDirection().x));
			dir.append(Json::Value(rigidbody->GetDirection().y));
			dir.append(Json::Value(rigidbody->GetDirection().z));
			Json::Value acc(Json::arrayValue);
			acc.append(Json::Value(rigidbody->GetAcceleration().x));
			acc.append(Json::Value(rigidbody->GetAcceleration().y));
			acc.append(Json::Value(rigidbody->GetAcceleration().z));
			Json::Value aabbMax(Json::arrayValue);
			aabbMax.append(Json::Value(rigidbody->GetAAbbMax().x));
			aabbMax.append(Json::Value(rigidbody->GetAAbbMax().y));
			aabbMax.append(Json::Value(rigidbody->GetAAbbMax().z));
			Json::Value aabbMin(Json::arrayValue);
			aabbMin.append(Json::Value(rigidbody->GetAAbbMin().x));
			aabbMin.append(Json::Value(rigidbody->GetAAbbMin().y));
			aabbMin.append(Json::Value(rigidbody->GetAAbbMin().z));

			root["RigidBody"]["Velocity"] = vel;
			root["RigidBody"]["Direction"] = dir;
			root["RigidBody"]["Acceleration"] = acc;
			root["RigidBody"]["AabbMax"] = aabbMax;
			root["RigidBody"]["AabbMin"] = aabbMin;

			root["RigidBody"]["Mass"] = rigidbody->GetMass();
			root["RigidBody"]["Friction"] = rigidbody->GetFriction();
			root["RigidBody"]["Knockback"] = rigidbody->GetKnockback();
			root["RigidBody"]["ID"] = rigidbody->GetID();
			root["RigidBody"]["IsActive"] = rigidbody->GetIsActive();
			root["RigidBody"]["Shape"] = "";

		}
		if (cmove)
		{
			root["CharacterMove"]["Speed"] = cmove->GetSpeed();
		}
		if (hp)
		{
			root["CharacterHP"]["m_HP"] = hp->GetHP();
		}
		if (phole)
		{
			Json::Value size(Json::arrayValue);
			size.append(Json::Value(phole->GetHoleSize().x));
			size.append(Json::Value(phole->GetHoleSize().y));
			root["HoleComponent"]["HoleSize"] = size;
		}

	}



	//void Factory::SaveEntity(Json::Value& root, VK::Entity* entity)
	//{
	//	//should be entity type
	//	root["EntityType"] = "entity type";
	//	root["Component"];
	//}
	void WriteToFile(const char* filename, Json::Value root)
	{
		std::ofstream file(filename);

		file << root;

		file.close();
	}






void LoadAudioData(std::string filename)
{
	VK::Application* app = VK::Application::GetAppInstance();
	VK::AudioSystem* m_pAS = app->getSystem<VK::AudioSystem>();
	//set file location
	std::string level = ReadFromFile("Audio\\AudioData\\" + filename);

	Json::Value root;
	Json::Reader reader;
	bool parsing = reader.parse(level, root);

	if (!parsing)
		return; 

	//audio part
	Json::Value audioPart = root["Audio"];
	std::vector<std::string> audioTypeList = audioPart.getMemberNames();

	
	VK::AudioType a_type;
	for (auto it : audioTypeList)
	{
		a_type = m_pAS->TranslateAudioKey(it);
		VK::AudioData *a_data= new VK::AudioData;
		a_data->filename = audioPart[it]["File"].asCString();
		a_data->volume = audioPart[it]["Volume"].asFloat();
		m_pAS->AddAudioDataList(a_type, a_data);
	}

}
void SaveAudioData(std::string filename)
{
	VK::Application* app = VK::Application::GetAppInstance();
	VK::AudioSystem* m_pAS = app->getSystem<VK::AudioSystem>();

	Json::Value audio_json;
	VK::AudioDataList audioList = m_pAS->GetAudioDataList();

	std::string enumstring;
	for (auto it : audioList)
	{
		enumstring = m_pAS->TranslateAudioKey(it.first);
		audio_json["Audio"][enumstring.c_str()]["File"] = it.second->filename.c_str();
		audio_json["Audio"][enumstring.c_str()]["Volume"] = it.second->volume;
	}
	std::string file = "Audio\\AudioData\\" + filename;
	WriteToFile(file.c_str(), audio_json);
}


void CreateMapEntity(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation)
{
	VK::Application* app = VK::Application::GetAppInstance();
	VK::EntitySystem* m_pES = app->getSystem<VK::EntitySystem>();

	VK::Entity * pMap = new VK::Entity;
	std::string number = std::to_string(mapID++);
	pMap->SetName("Map" + number);
	pMap->SetEntityType(VK::EntityType::E_MAP1);
	pMap->SetListType(VK::ListType::E_DYNAMIC);

	VK::TransformComponent* transform = new VK::TransformComponent;

	transform->setTranslation(pos);
	transform->setScale(scale);
	transform->setRotation(rotation);
	VK::MeshComponent* mesh = new VK::MeshComponent;
	mesh->setMesh("models/Terrain_Map_1.obj");


	VK::RigidBodyComponent* rigidybody = new VK::RigidBodyComponent;




	rigidybody->SetAAbbMax(glm::vec3(20.f));

	rigidybody->SetAAbbMin(glm::vec3(-20.f));


	transform->SetOwner(pMap);
	mesh->SetOwner(pMap);
	rigidybody->SetOwner(pMap);
	

	

	pMap->AddComponent("Transform", transform);
	pMap->AddComponent("MeshComponent", mesh);
	pMap->AddComponent("RigidBodyComponent", rigidybody);

	m_pES->AddEntity(pMap);

}
bool CreateItemEntity(glm::vec3 pos,VK::ItemType iType)
{
	if (!isOverlap(pos))
		return false;
	VK::Application* app = VK::Application::GetAppInstance();
	VK::EntitySystem* m_pES = app->getSystem<VK::EntitySystem>();
	VK::Entity * pItem = new VK::Entity;

	std::string number = std::to_string(item_ID++);

	pItem->SetName("ITEM" + number);
	pItem->SetEntityType(VK::EntityType::E_ITEM);
	pItem->SetListType(VK::ListType::E_DYNAMIC);

	VK::TransformComponent* transform = new VK::TransformComponent;

	transform->setTranslation(pos);
	transform->setScale(glm::vec3(7,7,7));
	transform->setRotation(glm::vec3(0, 0, 0));
	VK::MeshComponent* mesh = new VK::MeshComponent;
	switch (iType)
	{
	case VK::ItemType::E_ITEM_HP:
		mesh->setMesh("models/Item_HPup2.obj");
		break;
	case VK::ItemType::E_ITEM_SPEED_UP:
		mesh->setMesh("models/ItemSpeedUp.obj");
		break;
	case VK::ItemType::E_ITEM_SPEED_DOWN:
		mesh->setMesh("models/Item_SpeedDown.obj");
		break;
	default:
		break;
	}


	VK::RigidBodyComponent* rigidybody = new VK::RigidBodyComponent;

	rigidybody->SetAAbbMax(glm::vec3(15.f));
	rigidybody->SetAAbbMin(glm::vec3(-15.f));

	VK::ItemUsedComponent* itemUsed = new VK::ItemUsedComponent;
	itemUsed->SetItemUsed(false);
	itemUsed->SetItemType(iType);

	transform->SetOwner(pItem);
	mesh->SetOwner(pItem);
	rigidybody->SetOwner(pItem);
	itemUsed->SetOwner(pItem);

	pItem->AddComponent("Transform", transform);
	pItem->AddComponent("MeshComponent", mesh);
	pItem->AddComponent("RigidBodyComponent", rigidybody);
	pItem->AddComponent("ItemUsedComponent", itemUsed);


	m_pES->AddEntity(pItem);

	return true;
}
void CreateHoleEntity(glm::vec3 pos, glm::vec2 holeSize)
{
	VK::Application* app = VK::Application::GetAppInstance();
	VK::EntitySystem* m_pES = app->getSystem<VK::EntitySystem>();
	VK::Entity * pEntity = new VK::Entity;

	std::string number = std::to_string(item_ID++);

	pEntity->SetName("Hole" + number);
	pEntity->SetEntityType(VK::EntityType::E_HOLE);
	pEntity->SetListType(VK::ListType::E_DYNAMIC);

	VK::TransformComponent* transform = new VK::TransformComponent;
	transform->setTranslation(pos);
	transform->setScale(glm::vec3(10, 1, 10));
	transform->setRotation(glm::vec3(0, 0, 0));

	VK::MeshComponent* mesh = new VK::MeshComponent;
	mesh->setMesh("models/cube.obj");//change it into plane

	VK::HoleComponent* pHole = new VK::HoleComponent;
	pHole->SetHoleSize(holeSize);

	transform->SetOwner(pEntity);
	mesh->SetOwner(pEntity);
	pHole->SetOwner(pEntity);

	pEntity->AddComponent("Transform", transform);
	pEntity->AddComponent("MeshComponent", mesh);
	pEntity->AddComponent("HoleComponent", pHole);

	m_pES->AddEntity(pEntity);

}

	//todo time interval, list
void CreateItemVFX(VK::Entity* target, VK::ItemType iType)
{
	//VK::Application* app = VK::Application::GetAppInstance();
	//VK::EntitySystem* m_pES = app->getSystem<VK::EntitySystem>();
	//VK::Entity * pItem = new VK::Entity;

	//std::string number = std::to_string(item_ID++);

	//pItem->SetName("ITEM_VFX" + number);
	//pItem->SetEntityType(VK::EntityType::E_ITEM);
	//pItem->SetListType(VK::ListType::E_STATIC);

	//VK::TransformComponent* transform = new VK::TransformComponent;
	//VK::CharacterHP* characterHP = target->FindComponent<VK::CharacterHP>();
	//
	//VK::CharacterHP* hp = nullptr;
	//VK::TransformComponent* HPbar = nullptr;

	//VK::EntitySystem* p_entitySystem = VK::Application::GetAppInstance()->getSystem<VK::EntitySystem>();
	//VK::EntityListVector e_staticlist = p_entitySystem->GetStaticEntityList();
	//int i = 0, j = 0;
	//for (auto it : e_staticlist)
	//{
	//	if (it->GetName().find("HP_Bar") != std::string::npos)
	//	{
	//		HPbar = it->FindComponent<VK::TransformComponent>();
	//		j++;
	//	}
	//}

	//transform->setTranslation(HPbar->getTranslation() + glm::vec3(0.f, -3.f, 0.f));
	//transform->setScale(glm::vec3(7, 7, 7));
	//transform->setRotation(glm::vec3(0, 0, 0));
	//VK::MeshComponent* mesh = new VK::MeshComponent;
	//switch (iType)
	//{
	//case VK::ItemType::E_ITEM_HP:
	//	mesh->setMesh("models/Item_HPup2.obj");
	//	break;
	//case VK::ItemType::E_ITEM_SPEED_UP:
	//	mesh->setMesh("models/ItemSpeedUp.obj");
	//	break;
	//case VK::ItemType::E_ITEM_SPEED_DOWN:
	//	mesh->setMesh("models/Item_SpeedDown.obj");
	//	break;
	//default:
	//	break;
	//}

	//VK::ItemUsedComponent* itemUsed = new VK::ItemUsedComponent;
	//itemUsed->SetItemUsed(false);
	//itemUsed->SetItemType(iType);

	//transform->SetOwner(pItem);
	//mesh->SetOwner(pItem);
	//itemUsed->SetOwner(pItem);

	//pItem->AddComponent("Transform", transform);
	//pItem->AddComponent("MeshComponent", mesh);
	//pItem->AddComponent("ItemUsedComponent", itemUsed);


	//m_pES->AddEntity(pItem);
}


bool isOverlap(glm::vec3 pos)
{

	glm::vec3 aabbMax = glm::vec3(15.f, 15.f, 15.f);
	glm::vec3 aabbMin = glm::vec3(-15.f, -15.f, -15.f);

	VK::Application* app = VK::Application::GetAppInstance();
	VK::EntitySystem* m_pES = app->getSystem<VK::EntitySystem>();

	VK::EntityListVector e_DynamicList = m_pES->GetDynamicEntityList();


		for (auto it : e_DynamicList)
		{
			VK::ItemUsedComponent* itemUsed = it->FindComponent<VK::ItemUsedComponent>();

			VK::TransformComponent* p_TR = it->FindComponent<VK::TransformComponent>();
			VK::RigidBodyComponent* p_RB = it->FindComponent<VK::RigidBodyComponent>();
			VK::HoleComponent* p_Hole = it->FindComponent<VK::HoleComponent>();

			if (itemUsed)
			{
				if (!itemUsed->GetItemUsed())
				{
					if (VK::Geometry::AabbAabb(aabbMin + pos, aabbMax + pos, p_TR->getTranslation() + p_RB->GetAAbbMin(), p_TR->getTranslation() + p_RB->GetAAbbMax()))
					{
						return false;
					}
				}
			}
			if (p_Hole)
			{
				glm::vec3 HolePos = p_TR->getTranslation();
				glm::vec2 HoleRange = p_Hole->GetHoleSize() / 2.f;

				if (HolePos.x + HoleRange.x > pos.x && HolePos.x - HoleRange.x < pos.x)//x range check
					if (HolePos.z + HoleRange.y > pos.z  && HolePos.z - HoleRange.y < pos.z)//z range check
						return false;
			}
			
		}

	return true;
}

void CreateObstacleSnowballEntity(glm::vec3 pos, glm::vec3 vel)
{
	VK::Application* app = VK::Application::GetAppInstance();
	VK::EntitySystem* m_pES = app->getSystem<VK::EntitySystem>();
	VK::Entity * pEntity = new VK::Entity;


	std::string number = std::to_string(item_ID++);

	pEntity->SetName("Obstacle_Snow" + number);
	pEntity->SetEntityType(VK::EntityType::E_OBSTACLE_SNOWBALL);
	pEntity->SetListType(VK::ListType::E_DYNAMIC);

	VK::TransformComponent* transform = new VK::TransformComponent;
	transform->setTranslation(pos);
	transform->setScale(glm::vec3(25.f, 25.f, 25.f));
	transform->setRotation(glm::vec3(0, 0, 0));

	VK::MeshComponent* mesh = new VK::MeshComponent;
	mesh->setMesh("models/Snowball.obj");

	VK::RigidBodyComponent* rigid = new VK::RigidBodyComponent;
	rigid->SetVelocity(vel);
	rigid->SetAAbbMax(glm::vec3(20));
	rigid->SetAAbbMin(glm::vec3(-20));
	rigid->SetID(VK::BIG_SNOW);
	rigid->SetIsActive(true);

	VK::SnowballMove* snowballmove = new VK::SnowballMove;



	transform->SetOwner(pEntity);
	mesh->SetOwner(pEntity);

	pEntity->AddComponent("Transform", transform);
	pEntity->AddComponent("MeshComponent", mesh);
	pEntity->AddComponent("RigidBody", rigid);
	pEntity->AddComponent("SnowballMove", snowballmove);


	m_pES->AddEntity(pEntity);
}

}
