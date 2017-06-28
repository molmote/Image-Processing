/*****************************************************************************/
/*!
\file   Factory.h
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
#include "precompiled.h"
#include "json.h"


namespace VK
{
	class Entity;
	enum ItemType;
	class Component;
}
namespace Factory
{
	static int mapID = 0;
	static int item_ID = 0;

	std::string ReadFromFile(std::string filename);
	VK::Entity* CreateEntity(Json::Value entityPart, std::string e_name);
	VK::Component* CreateComponent(Json::Value componentPart, std::string componentType);
	void CreateLevel(std::string filename);


	void LevelSave(std::string filename);
	void WriteToFile(const char* filename, Json::Value root);
	void SaveComponent(Json::Value& root, VK::Entity* entity);
	//void Factory::SaveEntity(Json::Value& root,VK::Entity* entity);



	//Audio Part
	void LoadAudioData(std::string filename);
	void SaveAudioData(std::string filename);

	//create entity part
	void CreateMapEntity(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation);
	bool CreateItemEntity(glm::vec3 pos, VK::ItemType eType);
	void CreateHoleEntity(glm::vec3 pos, glm::vec2 holeSize);
	void CreateObstacleSnowballEntity(glm::vec3 pos, glm::vec3 vel);

	void CreateItemVFX(VK::Entity* target, VK::ItemType iType);

	bool isOverlap(glm::vec3 pos);

}
