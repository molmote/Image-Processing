/*****************************************************************************/
/*!
\file   Howtoplay.cpp
\author DoKyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   04/12/2017
\brief
This is the source file for Howtoplay state of Game play. When player enter 
into pause state, and selection box goes to How to play section, the How to 
play message came out.
Copyright 2017, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "HowtoPlay.h"
#include "Factory.h"
#include "Camera.h"
#include "InputSystem.h"
#include "StateSystem.h"
#include "EntitySystem.h"

HowtoPlay::HowtoPlay() : p_inputSystem(nullptr), p_stateSystem(nullptr), p_entitySystem(nullptr)
{
}

HowtoPlay::~HowtoPlay()
{

}

void HowtoPlay::Enter()
{
	p_inputSystem = m_app->getSystem<VK::InputSystem>();
	p_stateSystem = m_app->getSystem<VK::StateSystem>();
	p_entitySystem = m_app->getSystem<VK::EntitySystem>();
	Factory::CreateLevel("Howtoplay.Json");
	VK::Camera::GetInstance()->SetPosition(glm::vec3(0.f, 0.f, 240.f));
	VK::Camera::GetInstance()->SetFront(glm::vec3(0.f, 0.f, -1.f));
	prev = p_stateSystem->GetCurrentState();

	m_howtoplayTime = static_cast<float>(glfwGetTime());
}
void HowtoPlay::Update(float dt)
{
	if (prev == VK::STATE::STATE_PAUSE)
	{
		if (p_inputSystem->IsTriggered(VK::KEY_SPACE) || p_inputSystem->IsTriggered(VK::KEY_ENTER))
		{
			p_entitySystem->ClearDebugEntity();
			p_stateSystem->SetNextState(VK::STATE::STATE_PAUSE);
		}
	}
	else if (prev == VK::STATE::STATE_MAPSELECT /*&& something true*/)
	{
		float now = static_cast<float>(glfwGetTime());

		if (now - m_howtoplayTime > 5)
		{
			p_stateSystem->SetNextState(VK::STATE_GAME);
		}
		if (p_inputSystem->IsTriggered(VK::KEY_SPACE) || p_inputSystem->IsTriggered(VK::KEY_ENTER))
		{
			p_stateSystem->SetNextState(VK::STATE::STATE_GAME);
		}
	}
	else
	{
		if (p_inputSystem->IsTriggered(VK::KEY_SPACE) || p_inputSystem->IsTriggered(VK::KEY_ENTER))
		{
			p_stateSystem->SetNextState(VK::STATE::STATE_MENU);
		}
	}
	
}
void HowtoPlay::Exit()
{
	if(prev == VK::STATE::STATE_PAUSE)
		p_entitySystem->ClearDebugEntity();
	else
		p_entitySystem->ClearEntity();
}