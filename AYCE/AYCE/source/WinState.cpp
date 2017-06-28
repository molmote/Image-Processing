/*****************************************************************************/
/*!
\file   WinState.cpp
\author Dokyoom Kim
\par	email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   04/12/2017
\brief
source file for win state
Copyright 2017, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include "WinState.h"
#include "InputSystem.h"
#include "AudioSystem.h"
#include "StateSystem.h"
#include "Factory.h"
#include "EntitySystem.h"
#include "Camera.h"
#include "TransformComponent.h"

WinState::WinState()
{
	m_winState = VK::WINSTATE::NONE;
	p_particle = nullptr;
	m_cameraworks = true;
	m_Quit = 0;
	m_isQuit = false;
}
WinState::~WinState()
{

}

void WinState::Enter()
{
	p_inputSystem = m_app->getSystem<VK::InputSystem>();
	p_audioSystem = m_app->getSystem<VK::AudioSystem>();
	p_stateSystem = m_app->getSystem<VK::StateSystem>();
	p_entitySystem = m_app->getSystem<VK::EntitySystem>();
	p_particle = m_app->getSystem<VK::Particle>();
	Factory::CreateLevel("WinState.Json");

	VK::Camera::GetInstance()->SetPosition(glm::vec3(0, 800, 1000));	
	VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, -0.7, -1)));

	m_winState = p_stateSystem->GetWinState();
	m_winStateStartTime = static_cast<float>(glfwGetTime());
	SetWinner(m_winState);

	if (!p_audioSystem->IsSoundPlaying(VK::WINNING_BGM))
		p_audioSystem->Play_Sound(VK::WINNING_BGM, true);

	if (!p_audioSystem->IsSoundPlaying(VK::WINNING_SFX))
		p_audioSystem->Play_Sound(VK::WINNING_SFX, false);

	particle(10);
	m_Winselect = VK::WINSELECTION::BACKTOMENU;
	ShowWhatTodo(false);
}
void WinState::Update(float dt)
{ 
	float now = static_cast<float>(glfwGetTime());

	
	if (m_cameraworks == false)
	{
		switch (m_Winselect)
		{
		case VK::WINSELECTION::BACKTOMENU:
			if (p_inputSystem->IsTriggered(VK::KEY_ENTER))
			{
				p_stateSystem->SetNextState(VK::STATE_MENU);
			}
			break;
		case VK::WINSELECTION::BACKTOMAPSELECT:
			if (p_inputSystem->IsTriggered(VK::KEY_ENTER))
			{
				p_stateSystem->SetNextState(VK::STATE_MAPSELECT);
			}
			break;
		case VK::WINSELECTION::QUIT_GAME:
			if (p_inputSystem->IsTriggered(VK::KEY_ENTER))
			{
				VK::Camera::GetInstance()->SetPosition(glm::vec3(-800, -800, -700));
				VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, 0, -1)));

				m_isQuit = true;

				if (m_isQuit)
				{
					if (m_Quit == 1)
						m_app->SetAppActive(false);
					else if (m_Quit == 2)
					{
						ShowWhatTodo(true);
						m_Winselect = VK::WINSELECTION::BACKTOMENU;
						VK::Camera::GetInstance()->SetPosition(glm::vec3(-70, 0, 300));
						VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, 0, -1)));
						m_isQuit = false;
						m_Quit = 0;
					}
				}
			}
			if (p_inputSystem->IsTriggered(VK::KEY_ARROW_LEFT))
			{
				VK::EntityListVector e_staticlist = p_entitySystem->GetStaticEntityList();
				VK::TransformComponent* p_Yes, *p_No;

				for (auto it : e_staticlist)
				{
					if (it->GetName() == "Optionz2Yes")
						p_Yes = it->FindComponent<VK::TransformComponent>();
					if (it->GetName() == "Optionz3No")
						p_No = it->FindComponent<VK::TransformComponent>();
					}
				p_Yes->setScale(8);
				p_No->setScale(5);
				if (m_isQuit)
					m_Quit = 1;
			}
			if (p_inputSystem->IsTriggered(VK::KEY_ARROW_RIGHT))
			{
				VK::EntityListVector e_staticlist = p_entitySystem->GetStaticEntityList();
				VK::TransformComponent* p_Yes, *p_No;

				for (auto it : e_staticlist)
				{
					if (it->GetName() == "Optionz2Yes")
						p_Yes = it->FindComponent<VK::TransformComponent>();
					if (it->GetName() == "Optionz3No")
						p_No = it->FindComponent<VK::TransformComponent>();
				}
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
				if (m_Winselect == VK::WINSELECTION::BACKTOMENU)
				{
					m_Winselect = VK::WINSELECTION::BACKTOMAPSELECT;
					ChangeSelectionBox(VK::WINSELECTION::BACKTOMENU, m_Winselect);
				}
				else if (m_Winselect == VK::WINSELECTION::BACKTOMAPSELECT)
				{
					m_Winselect = VK::WINSELECTION::QUIT_GAME;
					ChangeSelectionBox(VK::WINSELECTION::BACKTOMAPSELECT, m_Winselect);
				}
			}
			if (p_inputSystem->IsTriggered(VK::KEY_ARROW_UP))
			{
				if (m_Winselect == VK::WINSELECTION::QUIT_GAME)
				{
					m_Winselect = VK::WINSELECTION::BACKTOMAPSELECT;
					ChangeSelectionBox(VK::WINSELECTION::QUIT_GAME, m_Winselect);
				}
				else if (m_Winselect == VK::WINSELECTION::BACKTOMAPSELECT)
				{
					m_Winselect = VK::WINSELECTION::BACKTOMENU;
					ChangeSelectionBox(VK::WINSELECTION::BACKTOMAPSELECT, m_Winselect);
				}
			}
		}
		
	}
	

	

	if (m_cameraworks)
	{
		if (p_inputSystem->IsTriggered(VK::KEY_ENTER))
		{
			ShowWhatTodo(true);
			VK::Camera::GetInstance()->SetPosition(glm::vec3(-70, 0, 150));
			VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, 0, -1)));
			m_cameraworks = false;
		}
		CameraWorks(dt);
	}
		
}
void WinState::Exit()
{
	p_audioSystem->Stop_Sound(VK::WINNING_BGM);

	p_entitySystem->ClearEntity();
	m_cameraworks = true;
}

void WinState::SetWinner(VK::WINSTATE state)
{
	switch (state)
	{
	case VK::PLAYER1_WIN:
	{
		VK::Entity * pEntity = new VK::Entity;
		pEntity->SetName("Player1 Win");
		pEntity->SetEntityType(VK::EntityType::E_UI);
		pEntity->SetListType(VK::ListType::E_STATIC);

		VK::TransformComponent* transform = new VK::TransformComponent;
		transform->setTranslation(glm::vec3(0,0,0));
		transform->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
		transform->setRotation(glm::vec3(0, 0, 0));

		VK::MeshComponent* mesh = new VK::MeshComponent;
		mesh->setMesh("models/character1.obj");

		transform->SetOwner(pEntity);
		mesh->SetOwner(pEntity);

		pEntity->AddComponent("Transform", transform);
		pEntity->AddComponent("MeshComponent", mesh);

		p_entitySystem->AddEntity(pEntity);
		break;
	}
	case VK::PLAYER2_WIN:
	{
		VK::Entity * pEntity = new VK::Entity;
		pEntity->SetName("Player2 Win");
		pEntity->SetEntityType(VK::EntityType::E_UI);
		pEntity->SetListType(VK::ListType::E_STATIC);

		VK::TransformComponent* transform = new VK::TransformComponent;
		transform->setTranslation(glm::vec3(0, 0, 0));
		transform->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
		transform->setRotation(glm::vec3(0, 0, 0));

		VK::MeshComponent* mesh = new VK::MeshComponent;
		mesh->setMesh("models/character2.obj");

		transform->SetOwner(pEntity);
		mesh->SetOwner(pEntity);

		pEntity->AddComponent("Transform", transform);
		pEntity->AddComponent("MeshComponent", mesh);

		p_entitySystem->AddEntity(pEntity);
		break;
	}
	default:
		break;
	}
}

void WinState::particle(int numbers)
{
	//fireworks
	for (int j = 0; j < numbers; ++j)
	{
		int v1 = rand() % 300 - 150;
		int v2 = rand() % 300 - 150;
		int v3 = 7 + rand() % 2;
		for (int i = 0; i < 20; ++i)
		{
			p_particle->CreateParticle(glm::vec3(v1, -250, v2), glm::vec3(0, 100, 0), glm::vec3(0, 0, 0), static_cast<VK::Particle_Type>(v3));
		}
	}
	
	
}
void WinState::CameraWorks(float dt)
{
	VK::Camera* camera = VK::Camera::GetInstance();
	glm::vec3 oripos, tarpos;
	glm::vec3 oridir, tardir;
	glm::vec3 diffPos, diffDir;

	oripos = camera->GetPosition();
	oridir = camera->GetFront();
	//tarpos = camera->GetPosition();
	//tardir = camera->GetFront();

	tarpos = glm::vec3(0, 200, 300);
	tardir = glm::vec3(0, -0.7, -1);

	diffPos = oripos - tarpos;
	diffPos = diffPos* (dt);
	diffDir = oridir - tardir;
	diffDir = diffDir* (dt);


	if (glm::length(diffPos) > 0.001)
		camera->GetInstance()->SetPosition(oripos - diffPos);
	if (glm::length(diffDir) > 0.001)
		camera->GetInstance()->SetFront(oridir - diffDir);
}

void WinState::ShowWhatTodo(bool truefalse)
{
	VK::EntityListVector e_staticlist = p_entitySystem->GetStaticEntityList();
	VK::TransformComponent* BackToMenu, *BackToGame, *QuitGame, *SelectionBox;

	for(auto it : e_staticlist)
	{ 
		if (it->GetName() == "BackToMenu")
			BackToMenu = it->FindComponent<VK::TransformComponent>();
		if (it->GetName() == "BackToGame")
			BackToGame = it->FindComponent<VK::TransformComponent>();
		if (it->GetName() == "QuitGame")
			QuitGame = it->FindComponent<VK::TransformComponent>();
		if (it->GetName() == "SelectionBox")
			SelectionBox = it->FindComponent<VK::TransformComponent>();
	}

	if (truefalse)
	{
		SelectionBox->setTranslation(-70, 20, 0);
		BackToMenu->setTranslation(-70, 20, 0);
		BackToGame->setTranslation(-70, 0, 0);
		QuitGame->setTranslation(-70, -20, 0);
	}
	else
	{
		SelectionBox->setTranslation(400, 400, 400);
		BackToMenu->setTranslation(400, 400, 400);
		BackToGame->setTranslation(400, 400, 400);
		QuitGame->setTranslation(400, 400, 400);
	}

}
void WinState::ChangeSelectionBox(int selection, int selctionGo)
{
	VK::EntityListVector e_staticlist = p_entitySystem->GetStaticEntityList();
	VK::TransformComponent *SelectionBox;

	for (auto it : e_staticlist)
	{
		if (it->GetName() == "SelectionBox")
			SelectionBox = it->FindComponent<VK::TransformComponent>();
	}
		
 	if ((selection - selctionGo)>0)
		SelectionBox->setTranslation(SelectionBox->getTranslation() + glm::vec3(0, 20, 0));
	else if ((selection - selctionGo)<0)
		SelectionBox->setTranslation(SelectionBox->getTranslation() - glm::vec3(0, 20, 0));
}