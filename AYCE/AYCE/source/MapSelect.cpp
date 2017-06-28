/*****************************************************************************/
/*!
\file   MapSelect.cpp
\author Dokyoom Kim
\par	email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   04/12/2017
\brief
source file for mapselect
Copyright 2017, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include "MapSelect.h"

#include "Application.h"
#include "InputSystem.h"
#include "AudioSystem.h"
#include "StateSystem.h"
#include "Factory.h"
#include "EntitySystem.h"
#include "Camera.h"
#include "TransformComponent.h"

MapSelect::MapSelect()
{
	m_mapState = static_cast<int>(VK::MAPSTATE::MAP_1);
}
MapSelect::~MapSelect()
{

}

void MapSelect::Enter()
{
	VK::Camera::GetInstance()->SetPosition(glm::vec3(0.f, 0.f, 210.f));
	VK::Camera::GetInstance()->SetFront(glm::vec3(0.f, 0.f, -1.f));

	p_inputSystem = m_app->getSystem<VK::InputSystem>();
	p_audioSystem = m_app->getSystem<VK::AudioSystem>();
	p_stateSystem = m_app->getSystem<VK::StateSystem>();
	p_entitySystem = m_app->getSystem<VK::EntitySystem>();
	m_mapState = static_cast<int>(VK::MAPSTATE::MAP_1);

	p_stateSystem->SetMapState(static_cast<VK::MAPSTATE>(abs(m_mapState)));

	Factory::CreateLevel("MapSelect.Json");


}
void MapSelect::Update(float dt)
{
	if (p_inputSystem->IsTriggered(VK::KEY_ARROW_DOWN) || p_inputSystem->IsTriggered(VK::KEY_ARROW_RIGHT))
	{
		m_mapState = (m_mapState + 1) % static_cast<int>(VK::MAPSTATE::MAP_MAX);
		p_stateSystem->SetMapState(static_cast<VK::MAPSTATE>(abs(m_mapState)));
	}
	else if (p_inputSystem->IsTriggered(VK::KEY_ARROW_LEFT) || p_inputSystem->IsTriggered(VK::KEY_ARROW_UP))
	{
		if (m_mapState == 0)
			m_mapState = 2;
		else
		m_mapState = (m_mapState - 1) % static_cast<int>(VK::MAPSTATE::MAP_MAX);

		p_stateSystem->SetMapState(static_cast<VK::MAPSTATE>(abs(m_mapState)));
	}
	std::cout << " Map State : " << m_mapState << std::endl;


	if (p_inputSystem->IsTriggered(VK::KEY_SPACE) || p_inputSystem->IsTriggered(VK::KEY_ENTER))
	{
		if (p_stateSystem->GetFirstTimePlaying())
		{
			p_stateSystem->SetNextState(VK::STATE::STATE_HOWTOPLAY);
			p_stateSystem->SetFirstTimePlaying(false);
		}
		else
		{
			p_stateSystem->SetNextState(VK::STATE::STATE_GAME);
		}
	}
	VK::EntityListVector e_staticlist = p_entitySystem->GetStaticEntityList();

	for (auto it : e_staticlist)
	{
		VK::TransformComponent* p_TR = it->FindComponent<VK::TransformComponent>();

		switch (m_mapState)
		{
		case VK::MAPSTATE::MAP_1:
			if (it->GetName() == "MapSelect1")
				p_TR->setTranslation(glm::vec3(0.f,0.f,0.f));
			else
				p_TR->setTranslation(glm::vec3(0.f, 0.f, -1000.f));
			break;
		case VK::MAPSTATE::MAP_2:
			if (it->GetName() == "MapSelect2")
				p_TR->setTranslation(glm::vec3(0.f, 0.f, 0.f));
			else
				p_TR->setTranslation(glm::vec3(0.f, 0.f, -1000.f));
			break;
		case VK::MAPSTATE::MAP_3:
			if (it->GetName() == "MapSelect3")
				p_TR->setTranslation(glm::vec3(0.f, 0.f, 0.f));
			else
				p_TR->setTranslation(glm::vec3(0.f, 0.f, -1000.f));
			break;

		}
	}
	

}
void MapSelect::Exit()
{
	p_entitySystem->ClearEntity();
}