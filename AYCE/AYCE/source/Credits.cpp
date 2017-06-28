/*****************************************************************************/
/*!
\file   Credits.cpp
\author Hyeongjin Park
\par    email: park.hyungjin\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the source file for Credit state of menu. When player enter into pause
state, Player can see game Credit
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "Credits.h"
#include "Factory.h"
#include "Camera.h"
#include "InputSystem.h"
#include "StateSystem.h"
#include "EntitySystem.h"
#include "AudioSystem.h"
#include "TransformComponent.h"

Credits::Credits(): p_inputSystem(nullptr), p_stateSystem(nullptr), p_entitySystem(nullptr)
{
	p_audioSystem = nullptr;
}

Credits::~Credits()
{

}

void Credits::Enter()
{

	VK::Camera::GetInstance()->SetPosition(glm::vec3(0, 400, 300));
	VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, 0.9 , -1)));
	p_inputSystem = m_app->getSystem<VK::InputSystem>();
	p_stateSystem = m_app->getSystem<VK::StateSystem>();
	p_entitySystem = m_app->getSystem<VK::EntitySystem>();
	p_audioSystem = m_app->getSystem<VK::AudioSystem>();

	Factory::CreateLevel("Credits.Json");
	p_audioSystem->Play_Sound(VK::CREDIT_BGM, true);

}
void Credits::Update(float dt)
{
	CameraWorks(dt*0.2f);
	if (p_inputSystem->IsTriggered(VK::KEY_ESCAPE)|| p_inputSystem->IsTriggered(VK::KEY_ENTER))
	{
		VK::Camera* camera = VK::Camera::GetInstance();
		p_stateSystem->SetNextState(VK::STATE_MENU);
		camera->GetInstance()->SetCameraState(VK::Camera_State::Start_pos);
	}
}
void Credits::Exit()
{
	p_audioSystem->Stop_Sound(VK::CREDIT_BGM);

	p_entitySystem->ClearEntity();
}
void Credits::CameraWorks(float dt) const
{
	VK::EntityListVector e_StaticList = p_entitySystem->GetStaticEntityList();
	
	for (auto it : e_StaticList)
	{
		it->FindComponent<VK::TransformComponent>()->setTranslation(it->FindComponent<VK::TransformComponent>()->getTranslation() + glm::vec3(0, 300, 0)*dt);
	}
	glm::vec3 pos = e_StaticList[0]->FindComponent<VK::TransformComponent>()->getTranslation();
	if(pos.y > 1200)
		p_stateSystem->SetNextState(VK::STATE_MENU);

}
