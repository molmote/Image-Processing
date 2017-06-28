/*****************************************************************************/
/*!
\file   PauseState.cpp
\author Hyeongjin Park
\par    email: park.hyungjin\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the source file for PauseState Menu. 
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "PauseState.h"
#include "Application.h"
#include "StateSystem.h"
#include "InputSystem.h"
#include "AudioSystem.h"
#include "EntitySystem.h"
#include "Factory.h"
#include "TransformComponent.h"
#include "Camera.h"
static void CameraMove(double&  startx, double& starty, bool& check);


PauseState::PauseState(): m_pauseState(0)
{
	p_inputSystem = nullptr;
	p_entitySystem = nullptr;
	p_audioSystem = nullptr;
	p_stateSystem = nullptr;
}

PauseState::~PauseState()
{
	
}

void PauseState::Enter()
{
	VK::Camera::GetInstance()->SetPosition(glm::vec3(-150, 0, 500));
	VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, 0, -1)));
	m_pauseState = 0;
	p_inputSystem = m_app->getSystem<VK::InputSystem>();
	p_entitySystem = m_app->getSystem<VK::EntitySystem>();
	p_audioSystem = m_app->getSystem<VK::AudioSystem>();
	p_stateSystem = m_app->getSystem<VK::StateSystem>();
	m_isQuit = false;
	
//	VK::Camera::GetInstance()->SetPosition(glm::vec3(0, 0, 400));
//	VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, 0, -1)));
	Factory::CreateLevel("Pause.Json");
	
	p_audioSystem->Stop_Sound_All();


}
void PauseState::Update(float dt)
{

	if (p_inputSystem->IsTriggered(VK::KEY_ESCAPE))
	{
		p_stateSystem->Resume();
		VK::Camera::GetInstance()->SetPosition(glm::vec3(0, 400, 600));
		VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, -0.9, -1)));
	}
	switch (m_pauseState)
	{
	case PAUSESTATE::RESUME:
		if (p_inputSystem->IsTriggered(VK::KEY_ENTER))
		{
			p_stateSystem->Resume();
			VK::Camera::GetInstance()->SetPosition(glm::vec3(0, 400, 600));
			VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, -0.9, -1)));
		}
		break;
	case PAUSESTATE::GAMERESTART:
		if (p_inputSystem->IsTriggered(VK::KEY_ENTER))
		{
			//p_entitySystem->ClearEntity();
			p_stateSystem->SetNextStateFromPause(VK::STATE_GAME);
		}
		break;
	case PAUSESTATE::OPTION:
		if (p_inputSystem->IsTriggered(VK::KEY_ENTER))
		{
			p_entitySystem->ClearDebugEntity();
			p_stateSystem->SetNextState(VK::STATE_OPTION);
		}
		break;
	case PAUSESTATE::BTM:
		if (p_inputSystem->IsTriggered(VK::KEY_ENTER))
			p_stateSystem->SetNextStateFromPause(VK::STATE_MENU);
		break;
	case PAUSESTATE::HTP:
		if (p_inputSystem->IsTriggered(VK::KEY_ENTER))
		{
			p_entitySystem->ClearDebugEntity();
			p_stateSystem->SetNextState(VK::STATE_HOWTOPLAY);
		}
		break;
	case PAUSESTATE::CREDITS:
		// do something here
		if (p_inputSystem->IsTriggered(VK::KEY_ENTER))
			p_stateSystem->SetNextStateFromPause(VK::STATE_CREDITS);

		//ShowHowToPlay(true);
		break;
	case PAUSESTATE::QUIT:
		if (p_inputSystem->IsTriggered(VK::KEY_ENTER))
		{
			VK::Camera::GetInstance()->SetPosition(glm::vec3(-400, -400, -300));
			VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, 0, -1)));
			m_isQuit = true;

			if (m_isQuit)
			{
				if (m_Quit == 1)
					m_app->SetAppActive(false);
				else if (m_Quit == 2)
				{
					VK::Camera::GetInstance()->SetPosition(glm::vec3(-150, 0, 500));
					VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, 0, -1)));
					m_isQuit = false;
					m_Quit = 0;
				}
			}

		}
		if (p_inputSystem->IsTriggered(VK::KEY_ARROW_LEFT))
		{
			VK::EntityListVector e_staticlist = p_entitySystem->GetDebugEntityList();
			VK::TransformComponent* p_Yes = e_staticlist[3]->FindComponent<VK::TransformComponent>();
			VK::TransformComponent* p_No = e_staticlist[4]->FindComponent<VK::TransformComponent>();
			p_Yes->setScale(8);
			p_No->setScale(5);
			if (m_isQuit)
				m_Quit = 1;
		}
		if (p_inputSystem->IsTriggered(VK::KEY_ARROW_RIGHT))
		{
			VK::EntityListVector e_staticlist = p_entitySystem->GetDebugEntityList();
			VK::TransformComponent* p_Yes = e_staticlist[3]->FindComponent<VK::TransformComponent>();
			VK::TransformComponent* p_No = e_staticlist[4]->FindComponent<VK::TransformComponent>();
			p_Yes->setScale(5);
			p_No->setScale(8);
			if (m_isQuit)
				m_Quit = 2;
		}
		break;
	}


	if (!m_isQuit)
	{
		if (p_inputSystem->IsTriggered(VK::KEY_ARROW_DOWN))
		{
			if (m_pauseState == PAUSESTATE::RESUME)
			{
				m_pauseState = PAUSESTATE::GAMERESTART;
				ChangeSelectionBox(PAUSESTATE::RESUME, m_pauseState);
			}
			else if (m_pauseState == PAUSESTATE::GAMERESTART)
			{
				m_pauseState = PAUSESTATE::OPTION;
				ChangeSelectionBox(PAUSESTATE::GAMERESTART, m_pauseState);
			}
			else if (m_pauseState == PAUSESTATE::OPTION)
			{
				m_pauseState = PAUSESTATE::BTM;
				ChangeSelectionBox(PAUSESTATE::OPTION, m_pauseState);
			}
			else if (m_pauseState == PAUSESTATE::BTM)
			{
				m_pauseState = PAUSESTATE::HTP;
				ChangeSelectionBox(PAUSESTATE::BTM, m_pauseState);
			}
			else if (m_pauseState == PAUSESTATE::HTP)
			{
				m_pauseState = PAUSESTATE::CREDITS;
				ChangeSelectionBox(PAUSESTATE::HTP, m_pauseState);
			}
			else if (m_pauseState == PAUSESTATE::CREDITS)
			{
				m_pauseState = PAUSESTATE::QUIT;
				ChangeSelectionBox(PAUSESTATE::CREDITS, m_pauseState);
			}
		}
		if (p_inputSystem->IsTriggered(VK::KEY_ARROW_UP))
		{
			if (m_pauseState == PAUSESTATE::GAMERESTART)
			{
				m_pauseState = PAUSESTATE::RESUME;
				ChangeSelectionBox(PAUSESTATE::GAMERESTART, m_pauseState);
			}
			else if (m_pauseState == PAUSESTATE::OPTION)
			{
				m_pauseState = PAUSESTATE::GAMERESTART;
				ChangeSelectionBox(PAUSESTATE::OPTION, m_pauseState);
			}
			else if (m_pauseState == PAUSESTATE::BTM)
			{
				m_pauseState = PAUSESTATE::OPTION;
				ChangeSelectionBox(PAUSESTATE::BTM, m_pauseState);
			}
			else if (m_pauseState == PAUSESTATE::HTP)
			{
				m_pauseState = PAUSESTATE::BTM;
				ChangeSelectionBox(PAUSESTATE::HTP, m_pauseState);
			}
			else if (m_pauseState == PAUSESTATE::CREDITS)
			{
				m_pauseState = PAUSESTATE::HTP;
				ChangeSelectionBox(PAUSESTATE::CREDITS, m_pauseState);
			}
			else if (m_pauseState == PAUSESTATE::QUIT)
			{
				m_pauseState = PAUSESTATE::CREDITS;
				ChangeSelectionBox(PAUSESTATE::QUIT, m_pauseState);
			}
		}
	}
}

void PauseState::Exit()
{
	p_entitySystem->ClearDebugEntity();
	if (p_stateSystem->GetNextState() == VK::STATE_GAME)
	{
		p_audioSystem->Play_Sound(VK::GAME_BGM, true);
	}

}
void PauseState::ChangeSelectionBox(int selection, int selctionGo)
{
	VK::EntityListVector e_staticlist = p_entitySystem->GetDebugEntityList();
	VK::TransformComponent* p_TR = e_staticlist[13]->FindComponent<VK::TransformComponent>();

	if ((selection - selctionGo)>0)
		p_TR->setTranslation(p_TR->getTranslation() + glm::vec3(0, 30, 0));
	else if ((selection - selctionGo)<0)
		p_TR->setTranslation(p_TR->getTranslation() - glm::vec3(0, 30, 0));
}

