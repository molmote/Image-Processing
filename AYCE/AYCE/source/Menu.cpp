/*****************************************************************************/
/*!
\file   Menu.cpp
\author Hyeongjin Park
\par    email: park.hyungjin\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the source file for Menu. Basically, game menu has Start, Option
Credits, Quit Game. When player controls(Arrow keys), the menu changes camera
setting to focus on where player heading at. For example, player controls 
menu as start play, the menu state autometically focus on(changes camera) start
play section.
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "Menu.h"
#include "Application.h"
#include "InputSystem.h"
#include "Factory.h"
#include "AudioSystem.h"
#include "Camera.h"
#include "ImguiManager.h"
#include "StateSystem.h"
#include "EntitySystem.h"
#include "Entity.h"
#include "Physics.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"
#include "Particle.h"
#include "GameLogic.h"


Menu::Menu()
{
	StateName = "Menutower";
	p_inputSystem = nullptr;
	p_entitySystem =nullptr;
	p_audioSystem = nullptr;
	p_physicsSystem = nullptr;
	p_imguiSystem = nullptr;
	p_stateSystem = nullptr;
	p_particle = nullptr;
	m_Quit = 0;
	m_isQuit = false;
	particleCounter = 0;
}
Menu::~Menu()
{
}

void Menu::Enter()
{
	VK::Camera::GetInstance()->SetCameraState(VK::Camera_State::Start_pos);
	p_inputSystem = m_app->getSystem<VK::InputSystem>();
	p_entitySystem = m_app->getSystem<VK::EntitySystem>();
	p_audioSystem = m_app->getSystem<VK::AudioSystem>();
	p_physicsSystem = m_app->getSystem<VK::PhysicsSystem>();
	p_imguiSystem = m_app->getSystem<VK::ImguiManager>();
	p_stateSystem = m_app->getSystem<VK::StateSystem>();
	p_particle = m_app->getSystem<VK::Particle>();
	p_gameLogic = m_app->getSystem<VK::GameLogic>();

	//glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)
	
	if (!p_audioSystem->IsSoundPlaying(VK::MENU_BGM))
		p_audioSystem->Play_Sound(VK::MENU_BGM, true);
	
	Factory::CreateLevel("Menu.Json");

	// changes a little bit
	// VK::Camera::GetInstance()->SetPosition(glm::vec3(-300, 450, 750));
	// changes nothing
	VK::Camera::GetInstance()->SetPosition(glm::vec3(-300, 400, 700));
	VK::Camera::GetInstance()->SetFront(glm::vec3(0.f, -0.5f, -1.f));

}
void Menu::Update(float dt)
{
	//system("cls");
	//std::cout<< p_entitySystem->GetStaticEntityList().size()<<std::endl;
	
	if (particleCounter < 50)
	{
		Particle();
		particleCounter++;
	}
	//std::cout << particleCounter;

	// we can control speed by calling CameraWorks function multiple times
	float speed = 2.5f;
	CameraWorks(VK::Camera::GetInstance()->GetCameraState(), dt*speed);
	p_particle->Update(dt);

	if (p_inputSystem->IsTriggered(VK::KEY_ENTER))
	{
		if (VK::Camera::GetInstance()->GetCameraState() == VK::Camera_State::Option_pos)
		{
			VK::Camera::GetInstance()->SetPosition(glm::vec3(0, 0, 600));
			VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, 0, -1)));
			p_stateSystem->SetNextState(VK::STATE_OPTION);
		}
		if (VK::Camera::GetInstance()->GetCameraState() == VK::Camera_State::Play_pos)
		{
			VK::Camera::GetInstance()->SetPosition(glm::vec3(0, 300, 600));
			VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, -0.7,-1)));
			//VK::Camera::GetInstance()->UpdateCamera();
			p_stateSystem->SetNextState(VK::STATE_MAPSELECT);
		}
		if (VK::Camera::GetInstance()->GetCameraState() == VK::Camera_State::Credit_pos)
		{
			VK::Camera::GetInstance()->SetPosition(glm::vec3(0, 500, 300));
			VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, 0.5, -1)));
			p_stateSystem->SetNextState(VK::STATE_CREDITS);
		}
		if (VK::Camera::GetInstance()->GetCameraState() == VK::Camera_State::Exit_pos)
		{
			VK::Camera::GetInstance()->SetPosition(glm::vec3(-400, -400, -300));
			VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, 0, -1)));
			m_isQuit = true;
			VK::Camera::GetInstance()->SetCameraState(VK::Camera_State::Exit_RUsure);
		}
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
				VK::Camera::GetInstance()->SetCameraState(VK::Camera_State::Start_pos);
			}
		}
	}

	if (!m_isQuit)
	{
		if (p_inputSystem->IsTriggered(VK::KEY_ARROW_DOWN) || p_inputSystem->IsTriggered(VK::KEY_ARROW_RIGHT) || p_inputSystem->IsTriggered(VK::KEY_ENTER))
			CameraMove(VK::KEY_ARROW_DOWN);
		if (p_inputSystem->IsTriggered(VK::KEY_ARROW_UP) || p_inputSystem->IsTriggered(VK::KEY_ARROW_LEFT))
			CameraMove(VK::KEY_ARROW_UP);
	}

	if (m_isQuit)
	{
		if (p_inputSystem->IsTriggered(VK::KEY_ARROW_LEFT))
		{
			VK::EntityListVector e_staticlist = p_entitySystem->GetStaticEntityList();
			VK::TransformComponent* p_Yes = e_staticlist[7]->FindComponent<VK::TransformComponent>();
			VK::TransformComponent* p_No = e_staticlist[8]->FindComponent<VK::TransformComponent>();
			p_Yes->setScale(8);
			p_No->setScale(5);
			if (m_isQuit)
				m_Quit = 1;
		}
		if (p_inputSystem->IsTriggered(VK::KEY_ARROW_RIGHT))
		{
			VK::EntityListVector e_staticlist = p_entitySystem->GetStaticEntityList();
			VK::TransformComponent* p_Yes = e_staticlist[7]->FindComponent<VK::TransformComponent>();
			VK::TransformComponent* p_No = e_staticlist[8]->FindComponent<VK::TransformComponent>();
			p_Yes->setScale(5);
			p_No->setScale(8);
			if (m_isQuit)
				m_Quit = 2;
		}
	}
	

}
void Menu::Exit()
{
	//Application* m_app = Application::GetAppInstance();
	p_entitySystem->ClearEntity();
	if(p_stateSystem->GetNextState() != VK::STATE_OPTION)
		p_audioSystem->Stop_Sound(VK::MENU_BGM); 
	StateName = "Menutower";
	p_gameLogic->ClearParticleList();
	p_particle->ClearParticle();
	particleCounter = 0;
}
void Menu::CameraWorks(int target, float dt)
{


	// within 0.5 secs, Change original lightDirection to target direction
	// and also change camera original to target
	p_entitySystem->GetStaticEntityList();
	
	VK::Camera* camera = VK::Camera::GetInstance();
	glm::vec3 oripos, tarpos;
	glm::vec3 oridir, tardir;
	//glm::vec3 orilight, tarlight;
	glm::vec3 diffPos, diffDir;

	oripos = camera->GetPosition();
	oridir = camera->GetFront();
	tarpos = camera->GetPosition();
	tardir = camera->GetFront();
	//orilight = 
	if (target == VK::Camera_State::Play_pos)
	{
		for (auto it : p_entitySystem->GetStaticEntityList())
		{
			if (it->GetName() == "MenuStart")
			{
				tarpos = it->FindComponent<VK::TransformComponent>()->getTranslation();
				tarpos += glm::vec3(0, 0, 150);
			}
		}
		tardir = glm::vec3(0, 0, -1);
		RotateParticle(VK::Camera_State::Play_pos);
	}
	if (target == VK::Camera_State::Option_pos)
	{
		for (auto it : p_entitySystem->GetStaticEntityList())
		{
			if (it->GetName() == "MenuOption")
			{
				tarpos = it->FindComponent<VK::TransformComponent>()->getTranslation();
				tarpos += glm::vec3(150, 0, 0);
			}
		}
		tardir = glm::vec3(-1, 0, 0);
		RotateParticle(VK::Camera_State::Option_pos);
	}
	if (target == VK::Camera_State::Credit_pos)
	{
		for (auto it : p_entitySystem->GetStaticEntityList())
		{
			if (it->GetName() == "MenuCredits")
			{
				tarpos = it->FindComponent<VK::TransformComponent>()->getTranslation();
				//????????
				//tarpos += glm::vec3(150, 0, 0);
			}
		}
		tarpos = glm::vec3(-60, 50, -150);
		tardir = glm::vec3(0, 0, 1);
		RotateParticle(VK::Camera_State::Credit_pos);
	}
	if (target == VK::Camera_State::Exit_pos)
	{
		tarpos = glm::vec3(-150, 45, 65);
		tardir = glm::vec3(1, 0, 0);
		RotateParticle(VK::Camera_State::Exit_pos);
	}
	if (target == VK::Camera_State::Start_pos)
	{
		tarpos = glm::vec3(-300, 400, 700);
		tardir = glm::vec3(0, -0.5, -1);
		RotateParticle(VK::Camera_State::Start_pos);
	}


	diffPos = oripos - tarpos;
	diffPos = diffPos* (dt);
	diffDir = oridir - tardir;
	diffDir = diffDir* (dt);


	if (glm::length(diffPos) > 0.003 )
		camera->GetInstance()->SetPosition(oripos - diffPos);
	if (glm::length(diffDir) > 0.003)
		camera->GetInstance()->SetFront(oridir - diffDir);
}
void Menu::CameraMove(int Key)
{
	VK::Camera* camera = VK::Camera::GetInstance();
	if (Key == VK::KEY_ARROW_DOWN  || Key == VK::KEY_ARROW_RIGHT)
	{
		if (StateName == "Menutower")
		{
			StateName = "MenuStart";
			p_audioSystem->Play_Sound(VK::MENU_MOVE_SFX, false);

			camera->GetInstance()->SetCameraState(VK::Camera_State::Play_pos);
			return;
		}
		if (StateName == "MenuStart")
		{
			StateName = "MenuOption";
			p_audioSystem->Play_Sound(VK::MENU_MOVE_SFX, false);

			camera->GetInstance()->SetCameraState(VK::Camera_State::Option_pos);
			return;
		}
		if (StateName == "MenuOption")
		{
			StateName = "Menu_Credits";
			p_audioSystem->Play_Sound(VK::MENU_MOVE_SFX, false);

			camera->GetInstance()->SetCameraState(VK::Camera_State::Credit_pos);
			return;
		}
		if (StateName == "Menu_Credits")
		{
			StateName = "Menu_Exit";
			p_audioSystem->Play_Sound(VK::MENU_MOVE_SFX, false);

			camera->GetInstance()->SetCameraState(VK::Camera_State::Exit_pos);
			return;
		}
	}
	if (Key == VK::KEY_ARROW_UP || Key == VK::KEY_ARROW_LEFT)
	{
		if (VK::Camera::GetInstance()->GetCameraState() == VK::Camera_State::Exit_RUsure)
		{
			return;
		}
		//VK::Camera* camera = VK::Camera::GetInstance();
		if (StateName == "MenuStart")
		{
			StateName = "Menutower";
			p_audioSystem->Play_Sound(VK::MENU_MOVE_SFX, false);

			/*camera->GetInstance()->SetPosition(glm::vec3(0, 300, 600));
			camera->GetInstance()->SetFront(glm::normalize(glm::vec3(0, -0.5, -1)));*/
			camera->GetInstance()->SetCameraState(VK::Camera_State::Start_pos);
			return;
		}
		if (StateName == "MenuOption")
		{
			StateName = "MenuStart";
			p_audioSystem->Play_Sound(VK::MENU_MOVE_SFX, false);

			/*camera->GetInstance()->SetPosition(glm::vec3(60, 65, 118));
			camera->GetInstance()->SetFront(glm::normalize(glm::vec3(0, 0, -1)));*/
			camera->GetInstance()->SetCameraState(VK::Camera_State::Play_pos);
			return;
		}
		if (StateName == "Menu_Credits")
		{
			StateName = "MenuOption";
			p_audioSystem->Play_Sound(VK::MENU_MOVE_SFX, false);

			/*camera->GetInstance()->SetPosition(glm::vec3(150, 55, -65));
			camera->GetInstance()->SetFront(glm::normalize(glm::vec3(-1, 0, 0)));*/
			camera->GetInstance()->SetCameraState(VK::Camera_State::Option_pos);
			return;
		}
		if (StateName == "Menu_Exit")
		{
			StateName = "Menu_Credits";
			p_audioSystem->Play_Sound(VK::MENU_MOVE_SFX, false);

			/*camera->GetInstance()->SetPosition(glm::vec3(-60, 50, -150));
			camera->GetInstance()->SetFront(glm::normalize(glm::vec3(0, 0, 1)));*/
			camera->GetInstance()->SetCameraState(VK::Camera_State::Credit_pos);
			return;
		}

	}
}
void Menu::Particle() 
{
	for (int i = 0; i < 4; ++i)
	{
		int v1 = rand() % 800 - 600;//-600 200
		int v2 = rand() % 200 + 600;// 600 800
		int v3 = rand() % 800 - 300;//-300 500
		p_particle->CreateParticle(glm::vec3(v1, v2, v3), static_cast<VK::Particle_Type>(rand() % 3));

	}
}
void Menu::RotateParticle(int camera)
{
	VK::EntityListVector e_StaticList = p_entitySystem->GetStaticEntityList();
	glm::vec3 rotate;
	rotate = glm::vec3(0, 0, 0);
	if (camera == VK::Camera_State::Play_pos)
		rotate = glm::vec3(0, 0, 0);
	if (camera == VK::Camera_State::Option_pos)
		rotate = glm::vec3(0, 1.57, 0);
	if (camera == VK::Camera_State::Credit_pos)
		rotate = glm::vec3(0, 3.14, 0);
	if (camera == VK::Camera_State::Exit_pos)
		rotate = glm::vec3(0, 4.71, 0);
	if (camera == VK::Camera_State::Start_pos)
		rotate = glm::vec3(0, 0, 0);


	for (auto it : e_StaticList)
	{
		VK::RigidBodyComponent* p_RB = it->FindComponent<VK::RigidBodyComponent>();
		VK::TransformComponent* p_TS = it->FindComponent<VK::TransformComponent>();

		if (p_TS && p_RB)
			if (p_RB->GetID() == VK::COMPONENTTYPE::PARTICLE)
				p_TS->setRotation(rotate);
	}
}