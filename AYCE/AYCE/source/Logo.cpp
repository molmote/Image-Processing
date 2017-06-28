/*****************************************************************************/
/*!
\file   Logo.cpp
\author Dokyoom Kim
\par	email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   04/12/2017
\brief
source file for logo state
Copyright 2017, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include "Logo.h"

#include "Application.h"
#include "InputSystem.h"
#include "AudioSystem.h"
#include "StateSystem.h"
#include "Factory.h"
#include "EntitySystem.h"
#include "Camera.h"
#include "TransformComponent.h"

Logo::Logo()
{
}
Logo::~Logo()
{

}

void Logo::Enter()
{
	p_inputSystem = m_app->getSystem<VK::InputSystem>();
	p_audioSystem = m_app->getSystem<VK::AudioSystem>();
	p_stateSystem = m_app->getSystem<VK::StateSystem>();
	p_entitySystem = m_app->getSystem<VK::EntitySystem>();
	Factory::CreateLevel("Logo.Json");
	VK::Camera::GetInstance()->SetPosition(glm::vec3(0.f, 0.f, 600.f));
	VK::Camera::GetInstance()->SetFront(glm::vec3(0.f, 0.f, -1.f));

	m_logoStartTime = static_cast<float>(glfwGetTime());


}
void Logo::Update(float dt)
{
	float now = static_cast<float>(glfwGetTime());
	VK::EntityListVector e_staticlist = p_entitySystem->GetStaticEntityList();

	if (now - m_logoStartTime > 3)
	{
		for (auto it : e_staticlist)
		{
			VK::TransformComponent* p_TR = it->FindComponent<VK::TransformComponent>();

			if (it->GetName() == "DigipenLogo")
			{
				p_TR->setTranslation(0.f, 0.f, -1000.f);
			}
			else if (it->GetName() == "MooLogo")
			{
				p_TR->setTranslation(0.f, 0.f, 0.f);
			}
		}
		if (now - m_logoStartTime > 6)
		{			
			p_stateSystem->SetNextState(VK::STATE::STATE_MENU);
		}
	}
	if (p_inputSystem->IsTriggered(VK::KEY_SPACE) || p_inputSystem->IsTriggered(VK::KEY_ENTER) || p_inputSystem->IsTriggered(VK::KEY_ESCAPE))
	{
		p_stateSystem->SetNextState(VK::STATE::STATE_MENU);
	}
}
void Logo::Exit()
{
	p_entitySystem->ClearEntity();

}