/*****************************************************************************/
/*!
\file   Game.cpp
\author Hyeongjin Park, Dokyoom Kim, Yeongki Baek
\par    email: park.hyungjin\@digipen.edu, yeongki.baek\@digipen.edu, 
\par	email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the source file for Credit state of menu. When player enter into pause
state, Player can see game Credit
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "Game.h"
#include "Application.h"
#include "InputSystem.h"
#include "Factory.h"
#include "AudioSystem.h"
#include "Camera.h"
#include "ImguiManager.h"
#include "StateSystem.h"
#include "GameLogic.h"
#include "Physics.h"
#include "imgui.h"
#include "CharacterHP.h"
#include "Utilities.h"
#include "Camera.h"
#include "SnowballMove.h"
#include "Particle.h"


namespace VK
{
	static void CameraMove(double&  startx, double& starty, bool& check);
	Game::Game() :
		m_startx(0),
		m_starty(0),
		check(false),
		audio_id(0),
		button(false),
		editor_button(false),
		m_Player1Invincible(false),
		m_Player2Invincible(false),
		m_Win(false),
		p_audioSystem(nullptr),
		p_inputSystem(nullptr),
		p_entitySystem(nullptr),
		p_physicsSystem(nullptr),
		p_imguiSystem(nullptr),
		p_stateSystem(nullptr)
	{
	}


	Game::~Game()
	{
	}

	void Game::Enter()
	{
		VK::Camera::GetInstance()->SetPosition(glm::vec3(0, 400, 600));
		VK::Camera::GetInstance()->SetFront(glm::normalize(glm::vec3(0, -0.9, -1)));

		m_Player1Invincible = false;
		m_Player2Invincible = false;
		p_inputSystem = m_app->getSystem<VK::InputSystem>();
		p_entitySystem = m_app->getSystem<VK::EntitySystem>();
		p_audioSystem = m_app->getSystem<VK::AudioSystem>();
		p_physicsSystem = m_app->getSystem<VK::PhysicsSystem>();
		p_imguiSystem = m_app->getSystem<VK::ImguiManager>();
		p_stateSystem = m_app->getSystem<VK::StateSystem>();
		p_gameLogic = m_app->getSystem<VK::GameLogic>();

		m_map = p_stateSystem->GetMapState();

		switch (m_map)
		{
		case VK::MAPSTATE::MAP_1:
			Factory::CreateLevel("Level1.Json");
			break;
		case VK::MAPSTATE::MAP_2:
			Factory::CreateLevel("Level2.Json");
			break;
		case VK::MAPSTATE::MAP_3:
			Factory::CreateLevel("Level3.Json");
			break;
		default:
			break;
		}

		//m_factory.LoadAudioData("AudioData.Json");
		m_startx = 0;
		m_starty = 0;
		m_Win = false;
		check = false;
		button = true;
		editor_button = false;
		m_startTime = static_cast<float>(glfwGetTime());
		m_windTime = static_cast<float>(glfwGetTime());
		m_snowballTime = static_cast<float>(glfwGetTime());
		p_audioSystem->Play_Sound(VK::GAME_BGM, true);
		if (p_audioSystem->IsMute())
			p_audioSystem->SetVolume(VK::GAME_BGM, 0.3f);
		p_gameLogic->SetLevelStartTime(static_cast<float>(glfwGetTime()));
		p_gameLogic->SetItemCreateTime(0.f);
	}
	void Game::Update(float dt)
	{
		VK::EntityListVector e_dynamiclist = p_entitySystem->GetDynamicEntityList();

		if(!player1 && !player2)
			for (auto it : e_dynamiclist)
			{
				if (it->GetName() == "Player1")
				{
					player1 = it;
				}
				else if(it->GetName() == "Player2")
				{
					player2 = it;
				}
			}

		if (m_map == VK::MAPSTATE::MAP_3)
		{
			float now = static_cast<float>(glfwGetTime());

			if (now - m_snowballTime > 10)
			{
				checkToStart = true;
			}

			if (checkToStart)
			{
				//m_snowballTime = static_cast<float>(glfwGetTime());

				if (checkObstacleType)
				{
					random = rand() % 4;
					std::cout << "random : " << random << std::endl;
					checkObstacleType = false;
				}
				glm::vec3 pos;

				switch (random)
				{
				case O_TYPE1:
				{
					pos.x = -215;
					pos.y = 15.f;
					pos.z = 215;
					glm::vec3 vel = glm::vec3(100.f, 0, 0);
					if (countObstacle < 5)
					{
						float currTime = static_cast<float>(glfwGetTime());
						if (currTime - m_snowballTime > 0.5)
						{
							m_snowballTime = static_cast<float>(glfwGetTime());
							pos.z -= 50.f*countObstacle;
							Factory::CreateObstacleSnowballEntity(pos, vel);
							countObstacle++;

						}
					}
					else
					{
						countObstacle = 0;
						checkToStart = false;
						m_snowballTime = static_cast<float>(glfwGetTime());
						checkObstacleType = true;

					}
					break;
				}
				case O_TYPE2:
				{
					pos.x = -215;
					pos.y = 15.f;
					pos.z = -215;
					glm::vec3 vel = glm::vec3(0, 0, 100.f);
					if (countObstacle < 5)
					{
						float currTime = static_cast<float>(glfwGetTime());
						if (currTime - m_snowballTime > 0.5)
						{
							m_snowballTime = static_cast<float>(glfwGetTime());
							pos.x += 50.f*countObstacle;
							Factory::CreateObstacleSnowballEntity(pos, vel);
							countObstacle++;

						}
					}
					else
					{
						countObstacle = 0;
						checkToStart = false;
						m_snowballTime = static_cast<float>(glfwGetTime());
						checkObstacleType = true;

					}
					break;
				}
				case O_TYPE3:
				{
					pos.x = 215;
					pos.y = 15.f;
					pos.z = -215;
					glm::vec3 vel = glm::vec3(0, 0, 100.f);
					if (countObstacle < 5)
					{
						float currTime = static_cast<float>(glfwGetTime());
						if (currTime - m_snowballTime > 0.5)
						{
							m_snowballTime = static_cast<float>(glfwGetTime());
							pos.x -= 50.f*countObstacle;
							Factory::CreateObstacleSnowballEntity(pos, vel);
							countObstacle++;

						}
					}
					else
					{
						countObstacle = 0;
						checkToStart = false;
						m_snowballTime = static_cast<float>(glfwGetTime());
						checkObstacleType = true;

					}
					break;
				}
				case O_TYPE4:
				{
					pos.x = -215;
					pos.y = 15.f;
					pos.z = -215;
					glm::vec3 vel = glm::vec3(100.f, 0, 0.f);
					if (countObstacle < 5)
					{
						float currTime = static_cast<float>(glfwGetTime());
						if (currTime - m_snowballTime > 0.5)
						{
							m_snowballTime = static_cast<float>(glfwGetTime());
							pos.z += 50.f*countObstacle;
							Factory::CreateObstacleSnowballEntity(pos, vel);
							countObstacle++;

						}
					}
					else
					{
						countObstacle = 0;
						checkToStart = false;
						m_snowballTime = static_cast<float>(glfwGetTime());
						checkObstacleType = true;

					}
					break;
				}

				}


			}





		}

		if (p_inputSystem->IsPressed(VK::KEY_LEFT_CONTROL) &&
			p_inputSystem->IsPressed(VK::KEY_LEFT_ALT))
		{
			// control alt 1 => player1 invincible
			if (p_inputSystem->IsTriggered(VK::KEY_1))
				m_Player1Invincible = !m_Player1Invincible;

			// control alt 2 => player2 invincible
			if (p_inputSystem->IsTriggered(VK::KEY_2))
				m_Player2Invincible = !m_Player2Invincible;

			// control alt 3 => kill player1
			if (p_inputSystem->IsTriggered(VK::KEY_3))
			{
				p_stateSystem->SetWinState(VK::PLAYER1_WIN);
				p_stateSystem->SetNextState(VK::STATE_WIN);
			}

			// control alt 4 => kill player2 
			if (p_inputSystem->IsTriggered(VK::KEY_4))
			{
				p_stateSystem->SetWinState(VK::PLAYER2_WIN);
				p_stateSystem->SetNextState(VK::STATE_WIN);
			}

			// control alt 5 => player1 autoplay
			if (p_inputSystem->IsPressed(VK::KEY_5))
			{
				p1_autoplay = !p1_autoplay;
			}

			// control alt 6 => player2 autoplay
			if (p_inputSystem->IsPressed(VK::KEY_6))
			{
				p2_autoplay = !p2_autoplay;
			}
		}

		if (p_inputSystem->IsTriggered(VK::KEY_ESCAPE))
		{
			p_stateSystem->PauseAndSetNext(VK::STATE_PAUSE);
		}
		else
		{
			HPbarScale();
			WinLostCondition();
			CameraMovement(dt);
		}
		if (p_inputSystem->IsPressed(VK::KEY_F1))
			p_stateSystem->SetNextState(VK::STATE_EDITOR);

		if (p_inputSystem->IsPressed(VK::KEY_ENTER))
		{
			if (m_Win)
				p_stateSystem->SetNextState(VK::STATE_MENU);;
		}

		//float windNow = static_cast<float>(glfwGetTime());

		//if (windNow - m_windTime > Utilities::RandMinMax(7, 12))
		//{
		//	p_audioSystem->Play_Sound(VK::WIND_SFX, false);
		//}

		if (p1_autoplay)
		{
			if (player1)
			{
				
				p1_current_time = glfwGetTime();
				if (p1_current_time - p1_last_time > p1_cooltime)
				{
					p1_direction = rand() % static_cast<int>(CharMoveFlag::CharMoveFlag_Size);
					p1_last_time = p1_current_time;
					p1_cooltime = Utilities::RandMinMax(0.5f, 1.5f);
				}
				CharMovement(player1, player1->FindComponent<CharacterMove>(), dt, static_cast<CharMoveFlag>(p1_direction));
				CreateSnowBall(player1);
			}
		}
		if(p2_autoplay)
		{
			if (player2)
			{
				p2_current_time = glfwGetTime();
				if (p2_current_time - p2_last_time > p2_cooltime )
				{
					p2_direction = rand() % static_cast<int>(CharMoveFlag::CharMoveFlag_Size);
					p2_last_time = p2_current_time;
					p2_cooltime = Utilities::RandMinMax(0.5f, 1.5f);
				}
				CharMovement(player2, player2->FindComponent<CharacterMove>(), dt, static_cast<CharMoveFlag>(p2_direction));
				CreateSnowBall(player2);
			}
		}
	}
	void Game::Exit()
	{
		p_audioSystem->Stop_Sound(VK::GAME_BGM);
		p_entitySystem->ClearEntity();
		p_gameLogic->ClearParticleList();

	}
	void Game::WinLostCondition()
	{

		VK::EntityListVector e_dynamiclist = p_entitySystem->GetDynamicEntityList();
		for (auto it : e_dynamiclist)
		{
			if (it->GetName() == "Player1")
			{
				VK::CharacterHP* pHp = it->FindComponent<VK::CharacterHP>();
				if (pHp->GetHP() <= 0.f)
				{
					p_stateSystem->SetWinState(VK::PLAYER2_WIN);
					p_stateSystem->SetNextState(VK::STATE_WIN);

				}
			}

			if (it->GetName() == "Player2")
			{
				VK::CharacterHP* pHp = it->FindComponent<VK::CharacterHP>();
				if (pHp->GetHP() <= 0.f)
				{
					p_stateSystem->SetWinState(VK::PLAYER1_WIN);
					p_stateSystem->SetNextState(VK::STATE_WIN);

				}
			}
		}
	}
	void Game::HPbarScale() const
	{
		VK::CharacterHP* hp[2] = { nullptr };
		VK::TransformComponent* HPbar[2] = { nullptr };
		VK::TransformComponent* HPbar_Real[2] = { nullptr };
		VK::TransformComponent* p_TR[2] = { nullptr };

		VK::EntityListVector e_dynamiclist = p_entitySystem->GetDynamicEntityList();
		VK::EntityListVector e_staticlist = p_entitySystem->GetStaticEntityList();
		int i = 0;
		for (auto it : e_dynamiclist)
		{
			if (it->GetName().find("Player") != std::string::npos)
			{
				hp[i] = it->FindComponent<VK::CharacterHP>();
				p_TR[i] = it->FindComponent<VK::TransformComponent>();
				++i;
			}
		}

		for (auto it : e_staticlist)
		{
			if (strcmp(it->GetName().c_str(), "Player1HP") == 0)
			{
				HPbar[0] = it->FindComponent<VK::TransformComponent>();
			}
			else if (strcmp(it->GetName().c_str(), "Player2HP") == 0)
			{
				HPbar[1] = it->FindComponent<VK::TransformComponent>();
			}
			else if (strcmp(it->GetName().c_str(), "Player1HP_Bar") == 0)
			{
				HPbar_Real[0] = it->FindComponent<VK::TransformComponent>();
			}
			else if (strcmp(it->GetName().c_str(), "Player2HP_Bar") == 0)
			{
				HPbar_Real[1] = it->FindComponent<VK::TransformComponent>();
			}
		}

		if (m_Player1Invincible)
			hp[0]->SetHP(100.f);

		if (m_Player2Invincible)
			hp[1]->SetHP(100.f);

		glm::vec3 HP1Scale = glm::vec3(3.5f);
		glm::vec3 HP2Scale = glm::vec3(3.5f);
		HP1Scale.x *= hp[0]->GetHP() / 100.f;
		HP2Scale.x *= hp[1]->GetHP() / 100.f;

		HPbar_Real[0]->setScale(HP1Scale);
		HPbar_Real[1]->setScale(HP2Scale);
		HPbar_Real[0]->setTranslation(p_TR[0]->getTranslation() + glm::vec3(-17.f, 50.f, 0.f));
		HPbar_Real[1]->setTranslation(p_TR[1]->getTranslation() + glm::vec3(-17.f, 50.f, 0.f));

		HPbar[0]->setTranslation(p_TR[0]->getTranslation() + glm::vec3(0.f, 50.f, 0.f));
		HPbar[1]->setTranslation(p_TR[1]->getTranslation() + glm::vec3(0.f, 50.f, 0.f));

	}
	void Game::CameraMovement(float dt)
	{
		VK::EntityListVector e_dynamiclist = p_entitySystem->GetDynamicEntityList();
		glm::vec3 PlayerPos[2];
		float DistancePlayers = 0.f;

		glm::vec3 oripos, tarpos, diffPos;
		glm::vec3 oridir;// , tardir, diffDir;

		int i = 0;
		for (auto it : e_dynamiclist)
		{
			if (it->GetName().find("Player") != std::string::npos)
			{
				PlayerPos[i] = it->FindComponent<VK::TransformComponent>()->getTranslation();
				i++;
			}
		}
		DistancePlayers = glm::distance(PlayerPos[0], PlayerPos[1]);
		static float PrevDistance = DistancePlayers;

		oripos = VK::Camera::GetInstance()->GetPosition();
		tarpos = VK::Camera::GetInstance()->GetPosition();

		oridir = VK::Camera::GetInstance()->GetFront();
		//tardir = VK::Camera::GetInstance()->GetFront();

		if (glm::abs(PrevDistance - DistancePlayers) >= 2.f)
		{
			//getting far away from each other
			if (PrevDistance - DistancePlayers <= 0)
				tarpos -= glm::normalize(oridir)*(DistancePlayers / 150.f * 20);
			//getting closer from each other	
			if (PrevDistance - DistancePlayers > 0)
				tarpos += glm::normalize(oridir)*(DistancePlayers / 150.f * 20);

			diffPos = oripos - tarpos;
			diffPos = diffPos* (dt);

			PrevDistance = DistancePlayers;
		}
		if (glm::length(diffPos) > 0.003 && (DistancePlayers < 400 && DistancePlayers > 100))
			VK::Camera::GetInstance()->SetPosition(oripos - diffPos);


		// camera fornt changes
	/*
		if (PrevDistance - DistancePlayers <= 0)
			tardir = glm::normalize(glm::vec3(0, -0.7, -1));
		if (PrevDistance - DistancePlayers > 0)
			tardir = glm::normalize(glm::vec3(0, -0.5, -1));

		diffDir = oridir - tardir;
		diffDir = diffDir* (dt);

		if (glm::length(diffDir) > 0.003)
			VK::Camera::GetInstance()->SetFront(oridir - diffDir);*/
	}



	void Game::CharMovement(VK::Entity* entity, VK::CharacterMove* p_CM, float dt, CharMoveFlag move_flag)
	{
		auto m_pAS = m_app->getSystem<AudioSystem>();
		VK::TransformComponent* transform = entity->FindComponent<VK::TransformComponent>();
		VK::RigidBodyComponent* rigid = entity->FindComponent<VK::RigidBodyComponent>();

		if (entity->GetEntityType() == EntityType::E_PLAYER1)
		{
			if (rigid->GetKnockback())
				return;

			{
				if (move_flag == CharMoveFlag::CharMoveFlag_UP)
				{
					p_CM->ReSetSnowball();
					if (p_CM->IsMovable(COLLISION_FLAG::COL_UP))
					{
						glm::vec3 pos = transform->getTranslation();
						pos.z -= p_CM->GetSpeed() *dt;
						transform->setTranslation(pos);
						transform->setRotation(C_UP);
						rigid->SetDirection(S_UP);
						p_CM->ResetCollisionFlag();
					}

					if (!m_pAS->IsSoundPlaying(WALKING_SFX))
						m_pAS->Play_Sound(WALKING_SFX, false);
					//CreatePaticle(entity, dt);
				}
				else if (move_flag == CharMoveFlag::CharMoveFlag_LEFT)
				{
					p_CM->ReSetSnowball();
					if (p_CM->IsMovable(COLLISION_FLAG::COL_LEFT))
					{
						glm::vec3 pos = transform->getTranslation();
						pos.x -= p_CM->GetSpeed()*dt;
						transform->setTranslation(pos);
						transform->setRotation(C_LEFT);
						rigid->SetDirection(S_LEFT);
						p_CM->ResetCollisionFlag();
					}

					if (!m_pAS->IsSoundPlaying(WALKING_SFX))
						m_pAS->Play_Sound(WALKING_SFX, false);
					//CreatePaticle(entity, dt);
				}
				else if (move_flag == CharMoveFlag::CharMoveFlag_DOWN)
				{
					p_CM->ReSetSnowball();
					if (p_CM->IsMovable(COLLISION_FLAG::COL_DOWN))
					{
						glm::vec3 pos = transform->getTranslation();
						pos.z += p_CM->GetSpeed()*dt;
						transform->setTranslation(pos);
						transform->setRotation(C_DOWN);
						rigid->SetDirection(S_DOWN);
						p_CM->ResetCollisionFlag();
					}
					if (!m_pAS->IsSoundPlaying(WALKING_SFX))
						m_pAS->Play_Sound(WALKING_SFX, false);
					//CreatePaticle(entity, dt);
				}
				else if (move_flag == CharMoveFlag::CharMoveFlag_RIGHT)
				{
					p_CM->ReSetSnowball();
					if (p_CM->IsMovable(COLLISION_FLAG::COL_RIGHT))
					{
						glm::vec3 pos = transform->getTranslation();
						pos.x += p_CM->GetSpeed()*dt;
						transform->setTranslation(pos);
						transform->setRotation(C_RIGHT);
						rigid->SetDirection(S_RIGHT);
						p_CM->ResetCollisionFlag();
					}

					if (!m_pAS->IsSoundPlaying(WALKING_SFX))
						m_pAS->Play_Sound(WALKING_SFX, false);
					//CreatePaticle(entity, dt);
				}
			}
		}


		if (entity->GetEntityType() == VK::EntityType::E_PLAYER2)
		{
			if (rigid->GetKnockback())
				return;
			{
				if (move_flag == CharMoveFlag::CharMoveFlag_UP)
				{
					p_CM->ReSetSnowball();
					if (p_CM->IsMovable(COLLISION_FLAG::COL_UP))
					{
						//todo asdjfkajdsflkjalsdkfjlsadkfj
						/*if (!p_CM->IsMovable(COLLISION_FLAG::COL_LEFT) && !p_CM->IsMovable(COLLISION_FLAG::COL_RIGHT))
						{
						glm::vec3 pos = transform->getTranslation();
						pos.z -= p_CM->GetSpeed()*dt;
						transform->setTranslation(pos);
						}*/
						glm::vec3 pos = transform->getTranslation();
						pos.z -= p_CM->GetSpeed()*dt;
						transform->setTranslation(pos);
						transform->setRotation(C_UP);
						rigid->SetDirection(S_UP);
						p_CM->ResetCollisionFlag();
					}
					if (!m_pAS->IsSoundPlaying(WALKING_SFX))
						m_pAS->Play_Sound(WALKING_SFX, false);
					//CreatePaticle(entity, dt);
				}
				else if (move_flag == CharMoveFlag::CharMoveFlag_LEFT)
				{
					p_CM->ReSetSnowball();
					if (p_CM->IsMovable(COLLISION_FLAG::COL_LEFT))
					{
						glm::vec3 pos = transform->getTranslation();
						pos.x -= p_CM->GetSpeed()*dt;
						transform->setTranslation(pos);
						transform->setRotation(C_LEFT);
						rigid->SetDirection(S_LEFT);
						p_CM->ResetCollisionFlag();
					}
					if (!m_pAS->IsSoundPlaying(WALKING_SFX))
						m_pAS->Play_Sound(WALKING_SFX, false);
					//CreatePaticle(entity, dt);
				}
				else if (move_flag == CharMoveFlag::CharMoveFlag_DOWN)
				{
					p_CM->ReSetSnowball();
					if (p_CM->IsMovable(COLLISION_FLAG::COL_DOWN))
					{
						glm::vec3 pos = transform->getTranslation();
						pos.z += p_CM->GetSpeed()*dt;
						transform->setTranslation(pos);
						transform->setRotation(C_DOWN);
						rigid->SetDirection(S_DOWN);
						p_CM->ResetCollisionFlag();
					}
					if (!m_pAS->IsSoundPlaying(WALKING_SFX))
						m_pAS->Play_Sound(WALKING_SFX, false);
					//CreatePaticle(entity, dt);
				}
				else if (move_flag == CharMoveFlag::CharMoveFlag_RIGHT)
				{
					p_CM->ReSetSnowball();
					if (p_CM->IsMovable(COLLISION_FLAG::COL_RIGHT))
					{
						glm::vec3 pos = transform->getTranslation();
						pos.x += p_CM->GetSpeed()*dt;
						transform->setTranslation(pos);
						transform->setRotation(C_RIGHT);
						rigid->SetDirection(S_RIGHT);
						p_CM->ResetCollisionFlag();
					}
					if (!m_pAS->IsSoundPlaying(WALKING_SFX))
						m_pAS->Play_Sound(WALKING_SFX, false);
					//CreatePaticle(entity, dt);
				}
			}
		}
	}
	void Game::CreateSnowBall(VK::Entity* entity)
	{
		auto m_pES = m_app->getSystem<EntitySystem>();
		auto m_pAS = m_app->getSystem<AudioSystem>();

		static double last_time_1 = 0.0;
		static double last_time_2 = 0.0;
		double cooltime1 = 1.0;
		double cooltime2 = 1.0;

		TransformComponent* entity_transform = entity->FindComponent<TransformComponent>();
		RigidBodyComponent* entity_rigid = entity->FindComponent<RigidBodyComponent>();
		CharacterMove* p_CM = entity->FindComponent<CharacterMove>();
		if (entity->GetName() == "Player1")
		{
			Entity* snowBall = p_CM->GetSnowball();
			if (snowBall)
				return;
			else if (!isOverlap(entity))
				return;
			double dt = glfwGetTime() - last_time_1;
			if (dt < cooltime1)
				return;
			Entity* nSnow = new Entity;
			std::string number = std::to_string(m_snowIDAuto++);
			nSnow->SetName("SnowBall" + number);
			nSnow->SetEntityType(E_SNOWBALL);
			nSnow->SetListType(E_DYNAMIC);
			m_pAS->Play_Sound(SHOOT_SFX, false);

			//transform
			TransformComponent* transform = new TransformComponent;
			glm::vec3 newPos = entity_rigid->GetDirection();
			newPos *= 12;
			transform->setTranslation(entity_transform->getTranslation() + newPos + glm::vec3(0, 5, 0));
			transform->setScale(5.f, 5.f, 5.f);
			MeshComponent* mesh = new MeshComponent;
			mesh->setMesh("models/Snowball.obj");
			RigidBodyComponent* rigid = new RigidBodyComponent;
			SnowballMove* snowballmove = new SnowballMove;

			transform->SetOwner(nSnow);
			mesh->SetOwner(nSnow);
			rigid->SetOwner(nSnow);
			rigid->SetID(COMPONENTTYPE::SNOW);
			snowballmove->SetOwner(nSnow);
			nSnow->AddComponent("Transform", transform);
			nSnow->AddComponent("MeshComponent", mesh);
			nSnow->AddComponent("RigidBody", rigid);
			nSnow->AddComponent("SnowballMove", snowballmove);
			m_pES->AddEntity(nSnow);
			p_CM->SetSnowball(nSnow);

			last_time_1 = glfwGetTime();

			Particle m_particle;
			m_particle.CreateParticle(entity_transform->getTranslation() + newPos + glm::vec3(0, 5, 0), Particle_Type::CreateSnow);


			AudioSystem* p_audioSystem = m_app->getSystem<AudioSystem>();
			p_audioSystem->Play_Sound(SHOOT_SFX, false);

			auto RbChar = player1->FindComponent<RigidBodyComponent>();

			rigid->SetIsActive(true);
			rigid->SetID(COMPONENTTYPE::SNOW1);
			rigid->SetVelocity(RbChar->GetDirection() *= 100);
			rigid->SetDirection(RbChar->GetDirection());

			p_CM->ReSetSnowball();
		}
		else if (entity->GetName() == "Player2")
		{
			//Particle m_particle(TrA->getTranslation(), glm::vec3(newvel), glm::vec3(0));

			Entity* snowBall = p_CM->GetSnowball();
			if (snowBall)
				return;
			else if (!isOverlap(entity))
				return;
			double dt = glfwGetTime() - last_time_2;
			if (dt < cooltime2)
				return;
			Entity* nSnow = new Entity;
			std::string number = std::to_string(m_snowIDAuto++);
			nSnow->SetName("SnowBall" + number);
			nSnow->SetEntityType(E_SNOWBALL);

			nSnow->SetListType(E_DYNAMIC);
			m_pAS->Play_Sound(SHOOT_SFX, false);
			//transform
			TransformComponent* transform = new TransformComponent;
			glm::vec3 newPos = entity_rigid->GetDirection();
			newPos *= 12;
			transform->setTranslation(entity_transform->getTranslation() + newPos + glm::vec3(0, 8, 0));
			transform->setScale(5.f, 5.f, 5.f);
			MeshComponent* mesh = new MeshComponent;
			mesh->setMesh("models/Snowball.obj");
			RigidBodyComponent* rigid = new RigidBodyComponent;
			SnowballMove* snowballmove = new SnowballMove;

			transform->SetOwner(nSnow);
			mesh->SetOwner(nSnow);
			rigid->SetOwner(nSnow);
			rigid->SetID(COMPONENTTYPE::SNOW);
			rigid->SetAAbbMax(glm::vec3(3));
			rigid->SetAAbbMin(glm::vec3(-3));
			snowballmove->SetOwner(nSnow);
			nSnow->AddComponent("Transform", transform);
			nSnow->AddComponent("MeshComponent", mesh);
			nSnow->AddComponent("RigidBody", rigid);
			nSnow->AddComponent("SnowballMove", snowballmove);
			m_pES->AddEntity(nSnow);
			p_CM->SetSnowball(nSnow);

			last_time_2 = glfwGetTime();

			Particle m_particle;
			m_particle.CreateParticle(entity_transform->getTranslation() + newPos + glm::vec3(0, 8, 0), Particle_Type::CreateSnow);

			AudioSystem* p_audioSystem = m_app->getSystem<AudioSystem>();
			p_audioSystem->Play_Sound(SHOOT_SFX, false);

			auto RbChar = player2->FindComponent<RigidBodyComponent>();

			rigid->SetIsActive(true);
			rigid->SetID(COMPONENTTYPE::SNOW2);
			rigid->SetVelocity(RbChar->GetDirection() *= 100);
			rigid->SetDirection(RbChar->GetDirection());

			p_CM->ReSetSnowball();
		}
	}

	bool Game::isOverlap(Entity* entity) const
	{
		auto m_pES = m_app->getSystem<EntitySystem>();

		glm::vec3 aabbMax = glm::vec3(10.1f, 10.1f, 10.1f);
		glm::vec3 aabbMin = glm::vec3(-10.1f, -10.1f, -10.1f);

		EntityListVector e_DynamicList = m_pES->GetDynamicEntityList();

		TransformComponent* p_ETR = entity->FindComponent<TransformComponent>();
		RigidBodyComponent* p_ERB = entity->FindComponent<RigidBodyComponent>();

		if (p_ERB)
		{
			for (auto it : e_DynamicList)
			{
				TransformComponent* p_TR = it->FindComponent<TransformComponent>();
				RigidBodyComponent* p_RB = it->FindComponent<RigidBodyComponent>();
				if (p_RB)
				{
					// whether character is 1 or 2
					if (p_RB->GetID() == 1 || p_RB->GetID() == 2)
						continue;

					glm::vec3 newPos = p_ERB->GetDirection();
					newPos *= 12;

					if (Geometry::AabbAabb(aabbMin + p_ETR->getTranslation() + newPos, aabbMax + p_ETR->getTranslation() + newPos,
						aabbMin + p_TR->getTranslation() + newPos, aabbMax + p_TR->getTranslation() + newPos))
						return false;
				}
			}
		}
		return true;
	}
}//namespace VK