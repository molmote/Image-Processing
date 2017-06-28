/*****************************************************************************/
/*!
\file   Option.cpp
\author Hyeongjin Park, Dokyoom Kim, Yeongki Baek
\par    email: park.hyungjin\@digipen.edu, yeongki.baek\@digipen.edu, 
\par	email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the source file for Option Menu. Basically, Option has some features 
such as sound, resolution and Quit. Sound has 10 steps of sound controls
Resolution has 4 steps.
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "Option.h"
#include "Factory.h" 
#include "InputSystem.h"
#include "StateSystem.h"
#include "EntitySystem.h"
#include "AudioSystem.h"
#include "Camera.h"
#include "TransformComponent.h"
#include "Entity.h"

namespace VK
{
	Option::Option(): m_OptionState(), m_volume(0), m_mute(false), m_fullScreen(false), m_resolution(), m_isQuit(false), m_Quit(0)
	{
		p_inputSystem = nullptr;
		p_stateSystem = nullptr;
		p_entitySystem = nullptr;
		p_audioSystem = nullptr;
	}

	Option::~Option()
	{

	}

	void Option::Enter()
	{
		m_resolution = m_app->m_currentResolution;
		VK::Camera::GetInstance()->SetPosition(glm::vec3(0, 0, 400));
		VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, 0, -1)));
		m_fullScreen = true;
		m_isQuit = false;
		m_Quit = 0;
		m_volume = 1.f;
		m_OptionState = OptionState::FirstFLAG;
		Factory::CreateLevel("Option.Json");
		p_inputSystem = m_app->getSystem<VK::InputSystem>();
		p_stateSystem = m_app->getSystem<VK::StateSystem>();
		p_entitySystem = m_app->getSystem<VK::EntitySystem>();
		p_audioSystem = m_app->getSystem<VK::AudioSystem>();


		VK::EntityListVector e_debugList = p_entitySystem->GetDebugEntityList();
		VK::TransformComponent* p_TRSoundBarReal = e_debugList[13]->FindComponent<VK::TransformComponent>();
		m_oriSoundBarSize = p_TRSoundBarReal->getScale();
		
		if(!p_audioSystem->IsSoundPlaying(VK::MENU_BGM))
			p_audioSystem->Play_Sound(VK::MENU_BGM, true);
		prev = p_stateSystem->GetCurrentState();
	}
	void Option::Update(float dt)
	{
		int number = p_entitySystem->GetDebugEntityList().size();
		std::cout << number << "!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
		StateofOption(m_OptionState);
		if (!m_isQuit)
		{
			if (p_inputSystem->IsTriggered(VK::KEY_ARROW_DOWN))
			{
				if (m_OptionState == OptionState::Back)
				{
					m_OptionState = OptionState::Sound;
					ChangeSelectionBox(OptionState::Back, m_OptionState);
				}
				else if (m_OptionState == OptionState::Sound)
				{
					m_OptionState = OptionState::Resolution;
					ChangeSelectionBox(OptionState::Sound, m_OptionState);
				}
				else if (m_OptionState == OptionState::Resolution)
				{
					m_OptionState = OptionState::Quit_;
					ChangeSelectionBox(OptionState::Resolution, m_OptionState);
				}
			}
			if (p_inputSystem->IsTriggered(VK::KEY_ARROW_UP))
			{
				if (m_OptionState == OptionState::Sound)
				{
					m_OptionState = OptionState::Back;
					ChangeSelectionBox(OptionState::Sound, m_OptionState);
				}
				else if (m_OptionState == OptionState::Resolution)
				{
					m_OptionState = OptionState::Sound;
					ChangeSelectionBox(OptionState::Resolution, m_OptionState);
				}
				else if (m_OptionState == OptionState::Quit_)
				{
					m_OptionState = OptionState::Resolution;
					ChangeSelectionBox(OptionState::Quit_, m_OptionState);
				}
			}
		}
		

		if (p_inputSystem->IsTriggered(VK::KEY_ESCAPE))
		{
			VK::Camera* camera = VK::Camera::GetInstance();
			p_stateSystem->SetNextState(VK::STATE_MENU);
			camera->GetInstance()->SetCameraState(VK::Camera_State::Start_pos);
		}
		if (p_inputSystem->IsTriggered(VK::KEY_ENTER))
		{
			if (m_OptionState == OptionState::Back)
			{
				if (prev == VK::STATE::STATE_PAUSE)
				{
					if (p_inputSystem->IsTriggered(VK::KEY_SPACE) || p_inputSystem->IsTriggered		(VK::KEY_ENTER))
					{
						p_entitySystem->ClearDebugEntity();
						p_stateSystem->SetNextState(VK::STATE::STATE_PAUSE);
					}
				}
				else
				{
					if (p_inputSystem->IsTriggered(VK::KEY_SPACE) || p_inputSystem->IsTriggered		(VK::KEY_ENTER))
					{
						p_stateSystem->SetNextState(VK::STATE::STATE_MENU);
					}
				}
			}
			else if (m_OptionState == OptionState::Sound)
			{

			}
			else if (m_OptionState == OptionState::Resolution)
			{
				VK::EntityListVector e_debugList = p_entitySystem->GetDebugEntityList();
				VK::TransformComponent* p_FullScreenoff = e_debugList[1]->FindComponent<VK::TransformComponent>();
				VK::TransformComponent* p_FullScreenon = e_debugList[2]->FindComponent<VK::TransformComponent>();
				
				

				if (m_fullScreen == false)
					m_fullScreen = true;
				else
					m_fullScreen = false;

				if (m_fullScreen)
				{
					p_FullScreenoff->setTranslation(400, 400, 400);
					p_FullScreenon->setTranslation(-200, 0, 0);
				} 
				else
				{
					p_FullScreenoff->setTranslation(-200, 0, 0);
					p_FullScreenon->setTranslation(400, 400, 400);
				}
				m_app->Recreate();
			}
			else if (m_OptionState == OptionState::Quit_)
			{
				VK::Camera::GetInstance()->SetPosition(glm::vec3(-400, -400, -300));
				VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, 0, -1)));
				m_isQuit = true;
				//m_app->SetAppActive(false);
			}
			if (m_isQuit)
			{
				if (m_Quit == 1)
					m_app->SetAppActive(false);
				else if (m_Quit == 2)
				{
					VK::Camera::GetInstance()->SetPosition(glm::vec3(0, 0, 400));
					VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, 0, -1)));
					m_isQuit = false;
					m_Quit = 0;
				}
			}
		}
		if (p_inputSystem->IsTriggered(VK::KEY_ARROW_LEFT))
		{
			if (m_OptionState == OptionState::Sound)
			{
				if (m_volume > 0.f)
					m_volume -= 0.1f;

				p_audioSystem->VolumeDown_All();
				
				ChangeSound();
			}
			else if (m_OptionState == OptionState::Resolution)
			{
				if (m_resolution == ResolutionType::by800600)
				{
					m_resolution = ResolutionType::by19201080;
					ShowupResolution(m_resolution);
				}
				else if (m_resolution == ResolutionType::by19201080)
				{
					m_resolution = ResolutionType::by1600900;
					ShowupResolution(m_resolution);
				}
				else if (m_resolution == ResolutionType::by1600900)
				{
					m_resolution = ResolutionType::by1280720;
					ShowupResolution(m_resolution);
				}
				else if (m_resolution == ResolutionType::by1280720)
				{
					m_resolution = ResolutionType::by800600;
					ShowupResolution(m_resolution);
				}
				VK::Application::GetAppInstance()->SetCurrentResolution(m_resolution);
			}
			else if (m_OptionState == OptionState::Quit_)
			{
				VK::EntityListVector e_debugList = p_entitySystem->GetDebugEntityList();
				VK::TransformComponent* p_Yes = e_debugList[15]->FindComponent<VK::TransformComponent>();
				VK::TransformComponent* p_No = e_debugList[16]->FindComponent<VK::TransformComponent>();
				p_Yes->setScale(8);
				p_No->setScale(5);
				if(m_isQuit)
					m_Quit = 1;
			}
		}
		if (p_inputSystem->IsTriggered(VK::KEY_ARROW_RIGHT))
		{
			if (m_OptionState == OptionState::Sound)
			{
				if (m_volume < 1.f)
					m_volume += 0.1f;

				p_audioSystem->VolumeUp_All();

				ChangeSound();
			}
			else if (m_OptionState == OptionState::Resolution)
			{

				if (m_resolution == ResolutionType::by19201080)
				{
					m_resolution = ResolutionType::by800600;
					ShowupResolution(m_resolution);
				}
				else if (m_resolution == ResolutionType::by1600900)
				{
					m_resolution = ResolutionType::by19201080;
					ShowupResolution(m_resolution);
				}
				else if (m_resolution == ResolutionType::by1280720)
				{
					m_resolution = ResolutionType::by1600900;
					ShowupResolution(m_resolution);
				}
				else if (m_resolution == ResolutionType::by800600)
				{
					m_resolution = ResolutionType::by1280720;
					ShowupResolution(m_resolution);
				}
				VK::Application::GetAppInstance()->SetCurrentResolution(m_resolution);
			}
			else if (m_OptionState == OptionState::Quit_)
			{
				VK::EntityListVector e_debugList = p_entitySystem->GetDebugEntityList();
				VK::TransformComponent* p_Yes = e_debugList[15]->FindComponent<VK::TransformComponent>();
				VK::TransformComponent* p_No = e_debugList[16]->FindComponent<VK::TransformComponent>();
				p_Yes->setScale(5);
				p_No->setScale(8);
				if (m_isQuit)
					m_Quit = 2;
			}
		}

	}
	void Option::Exit()
	{
		if (prev == VK::STATE::STATE_PAUSE)
			p_entitySystem->ClearDebugEntity();
		else
			p_entitySystem->ClearEntity();
	}
	void Option::ChangeSelectionBox(OptionState selection, OptionState selctionGo)
	{
		VK::EntityListVector e_debugList = p_entitySystem->GetDebugEntityList();
		VK::TransformComponent* p_TR = e_debugList[10]->FindComponent<VK::TransformComponent>();

		if ((static_cast<int>(selection) - static_cast<int>(selctionGo)) > 0)
			p_TR->setTranslation(p_TR->getTranslation() + glm::vec3(0, 40, 0));
		else if ((static_cast<int>(selection) - static_cast<int>(selctionGo)) < 0)
			p_TR->setTranslation(p_TR->getTranslation() - glm::vec3(0, 40, 0));
	}
	void Option::StateofOption(OptionState state)
	{
		VK::EntityListVector e_debugList = p_entitySystem->GetDebugEntityList();
		VK::TransformComponent* p_TRSoundBar = e_debugList[12]->FindComponent<VK::TransformComponent>();
		VK::TransformComponent* p_TRSoundBarReal = e_debugList[13]->FindComponent<VK::TransformComponent>();
		switch (state)
		{
		case OptionState::Back:
			p_TRSoundBar->setTranslation(glm::vec3(400, 400, 400));
			p_TRSoundBarReal->setTranslation(glm::vec3(400, 400, 400));
			break;
		case OptionState::Sound:
			p_TRSoundBar->setTranslation(e_debugList[10]->FindComponent<VK::TransformComponent>()->getTranslation() + glm::vec3(200, 0, 0));
			p_TRSoundBarReal->setTranslation(e_debugList[10]->FindComponent<VK::TransformComponent>()->getTranslation() + glm::vec3(130, 0, 0));
			ShowupResolution(ResolutionType::LASTFLAG);
			break;
		case OptionState::Resolution:
			ShowupResolution(m_resolution);
			p_TRSoundBar->setTranslation(glm::vec3(400, 400, 400));
			p_TRSoundBarReal->setTranslation(glm::vec3(400, 400, 400));
			break;
		case OptionState::Quit_:
			ShowupResolution(ResolutionType::LASTFLAG);
			break;
		}
	}
	void Option::ChangeSound()
	{
		VK::EntityListVector e_debugList = p_entitySystem->GetDebugEntityList();
		VK::TransformComponent* p_TRSoundBarReal = e_debugList[13]->FindComponent<VK::TransformComponent>();
		glm::vec3 SoundbarScale = glm::vec3(14, 14, 12);
		SoundbarScale.x *= m_volume / 1;
		
		p_TRSoundBarReal->setScale(SoundbarScale);
	}
	void Option::ShowupResolution(ResolutionType resolution)
	{
		VK::EntityListVector e_debugList = p_entitySystem->GetDebugEntityList();
//		VK::TransformComponent* P_TRRes;

		for (auto & it : e_debugList)
		{
			//s1.find(s2) != std::string::npos
			if (it->GetName().find("by") != std::string::npos)
			{
				VK::TransformComponent* P_TRRes = it->FindComponent<VK::TransformComponent>();
				P_TRRes->setTranslation(glm::vec3(400, 400, 400));
			}
		}

		if (resolution == ResolutionType::by19201080)
		{
			e_debugList[7]->FindComponent<VK::TransformComponent>()->setTranslation(e_debugList[10]->FindComponent<VK::TransformComponent>()->getTranslation() + glm::vec3(200, 0, 0));
		}
		else if (resolution == ResolutionType::by1600900)
		{
			e_debugList[6]->FindComponent<VK::TransformComponent>()->setTranslation(e_debugList[10]->FindComponent<VK::TransformComponent>()->getTranslation() + glm::vec3(200, 0, 0));
		}
		else if (resolution == ResolutionType::by1280720)
		{
			e_debugList[5]->FindComponent<VK::TransformComponent>()->setTranslation(e_debugList[10]->FindComponent<VK::TransformComponent>()->getTranslation() + glm::vec3(200, 0, 0));
		}
		else if (resolution == ResolutionType::by800600)
		{
			e_debugList[8]->FindComponent<VK::TransformComponent>()->setTranslation(e_debugList[10]->FindComponent<VK::TransformComponent>()->getTranslation() + glm::vec3(200, 0, 0));
		}
	}
} // namespace VK;