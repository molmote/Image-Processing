/*****************************************************************************/
/*!
\file   GameLogic.cpp
\author Hyeongjin Park, Dokyoom Kim, Yeongki Baek
\par    email: park.hyungjin\@digipen.edu, yeongki.baek\@digipen.edu,
\par	email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for EntitySystem features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "GameLogic.h"
#include "Entity.h"
#include "EntitySystem.h"
#include "AudioSystem.h"
#include "InputSystem.h"
#include "CharacterMove.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "MeshComponent.h"
#include "SnowballMove.h"
#include "ItemUsedComponent.h"
#include "Geometry.h"
#include "Camera.h"
#include "StateSystem.h"
#include "Utilities.h"
#include "Particle.h"
#include "Factory.h"
#include "CharacterHP.h"
#include "HoleComponent.h"
//#include "Controller.h"

namespace VK
{

	GameLogic::GameLogic()
	{ 
		m_pES = nullptr;
		m_pIS = nullptr;
		m_pAS = nullptr;
		m_pStateSystem = nullptr;
		m_snowID = 0;
		m_levelStartTime = 0.f;
		m_createTime = 0.f;
		m_particle = nullptr;
		total_item = 0;
		m_rightOrLeft = false;
	}
	GameLogic::~GameLogic()
	{

	}

	void GameLogic::Init()
	{
		START();
		m_pES = m_app->getSystem<EntitySystem>();
		m_pIS = m_app->getSystem<InputSystem>();
		m_pAS = m_app->getSystem<AudioSystem>();
		m_pStateSystem = m_app->getSystem<StateSystem>();
		m_particle = m_app->getSystem<VK::Particle>();

		LOG("Set member pointers");
	}
	void GameLogic::Update(float dt)
	{
		//m_particle->Update(dt);
		EntityListVector e_DynamicList = m_pES->GetDynamicEntityList();
		STATE currState = m_pStateSystem->GetCurrentState();

		if (currState != STATE_PAUSE)
		{


			for (auto it : e_DynamicList)
			{
				CharacterMove* p_CM = it->FindComponent<CharacterMove>();
				SnowballMove* p_SM = it->FindComponent<SnowballMove>();
				TransformComponent* p_TR = it->FindComponent<TransformComponent>();
				RigidBodyComponent* p_RB = it->FindComponent<RigidBodyComponent>();
				ItemUsedComponent* pItemUsed = it->FindComponent<ItemUsedComponent>();

				//if item is created
				if (pItemUsed)
				{
					//if item is used delete it
					if (pItemUsed->GetItemUsed())
					{
						m_pES->DeleteEntity(it);
						--total_item;
					}
					else
					{
						glm::vec3 rot = p_TR->getRotation();
						rot.y += dt;
						p_TR->setRotation(glm::vec3(0.f, rot.y, 0.f));

					}



				}


				if (currState == STATE_GAME)
				{
					float currentTime = static_cast<float>(glfwGetTime());
					if ((currentTime - m_levelStartTime) > 10)
					{

						if (currentTime - m_createTime > 8 && total_item <5)
						{
							m_createTime = static_cast<float>(glfwGetTime());
							//create HP Items here
							glm::vec3 pos;
							pos.x = static_cast<float>(Utilities::RandMinMax(-200, 200));
							pos.y = 10.f;
							pos.z = static_cast<float>(Utilities::RandMinMax(-200, 200));
							int item_type = 0;
							item_type = Utilities::RandMinMax(0, E_ITEM_LAST);
							if (Factory::CreateItemEntity(pos, static_cast<ItemType>(item_type)))
								total_item++;
						}
					}

				}


				if (p_RB)
				{
					if (p_CM)
					{
						
						//when character is in hole
						if (p_CM->GetFallState())
						{
							if (p_RB->GetVelocity() == glm::vec3(0.f))
								p_RB->SetVelocity(p_RB->GetDirection() * 100.f);
							else
							{
								p_RB->SetVelocity(p_RB->GetVelocity() * 0.99f - glm::vec3(0.f, 20.f, 0.f));
							}
							//p_RB->SetVelocity(glm::vec3(0.f, 3.f, 0.f));
							if (p_TR->getTranslation().y < -400.f)
							{
								auto pHp = it->FindComponent<VK::CharacterHP>();
								
								m_pAS->Fallcheck = true;
								pHp->SetHP(0.f);
							}
						}
						else
						{
							//todo asdf
							//RotateCharacter(it, dt);
							CharMovement(it, p_CM, dt);
							CreateSnowBall(it);
						}



					}

					if (p_SM)
					{
						if (p_SM->GetFallState())
						{

							if (p_RB->GetVelocity() == glm::vec3(0.f))
								p_RB->SetVelocity(p_RB->GetDirection() * 100.f);
							else
							{
								p_RB->SetVelocity(p_RB->GetVelocity() * 1.01f - glm::vec3(0.f, 20.f, 0.f));
							}

							if(p_TR->getTranslation().y <-600.f)
								m_pES->DeleteEntity(it);

						}
						else
						{
							ShootSnowBall(it, dt);
							ScaleSnowBall(it, dt);
							RotateSnowBall(it, dt);
						}
					}

					//todo should be removed
					//if (p_TR->getTranslation().x > 235.f ||
					//	p_TR->getTranslation().x < -235.f ||
					//	p_TR->getTranslation().z > 235.f ||
					//	p_TR->getTranslation().z < -235.f)

					//{
					//	if (p_RB->GetID() == COMPONENTTYPE::SNOW1 || p_RB->GetID() == COMPONENTTYPE::SNOW2)
					//		m_pES->DeleteEntity(it);
					//	/*else if (p_RB->GetID() == COMPONENTTYPE::CHARACTER1 || p_RB->GetID() == COMPONENTTYPE::CHARACTER2)
					//	{
					//		p_RB->SetVelocity(glm::vec3(0));
					//	}*/

					//}
				}
			}
		}
		else
		{

		}
	}
	void GameLogic::Shutdown()
	{


	}
	void GameLogic::GetSnowballPos(float dt)
	{
		static float particleTime = 0;
		particleTime += dt;

		if (particleTime > 0.6)
		{
			VK::EntityListVector Snowballlist = m_pES->GetDynamicEntityList();
			for (auto it : Snowballlist)
			{
				if (it->GetEntityType() == EntityType::E_SNOWBALL)
				{
					if (it->FindComponent<RigidBodyComponent>()->GetIsActive())
					{
						glm::vec3 pos = it->FindComponent<TransformComponent>()->getTranslation();
						m_particlePos.push_back(pos);
					}
				}
			}

			particleTime = 0;
			return;
		}
	}
	void GameLogic::GetPlayersPos(float dt)
	{

		static float particleTime = 0;
		particleTime += dt;

		if (particleTime > 0.6)
		{
			VK::EntityListVector Playerlist = m_pES->GetDynamicEntityList();
			for (auto it : Playerlist)
			{
				if (it->GetEntityType() == EntityType::E_PLAYER1)
				{
					m_particleP1Pos.push_back(it->FindComponent<TransformComponent>()->getTranslation());
				}
				if (it->GetEntityType() == EntityType::E_PLAYER2)
				{
					m_particleP2Pos.push_back(it->FindComponent<TransformComponent>()->getTranslation());
				}
			}

			m_rightOrLeft = !m_rightOrLeft;
			particleTime = 0;
			return;
		}
		
	}

	void GameLogic::CreatePaticle(float dt)
	{
		VK::CharacterHP* hp[2] = { nullptr };
		VK::EntityListVector Playerlist = m_pES->GetDynamicEntityList();
		glm::vec3 rotation1, rotation2;
		static Particle_Type particleType = Particle_Type::Walk_Particle_1;

		// show particles 
		int i = 0, j = 0;
		for (auto it : Playerlist)
		{
			if (it->GetName().find("Player") != std::string::npos)
			{
				hp[i] = it->FindComponent<VK::CharacterHP>();
				i++;
			}
			if (it->GetEntityType() == EntityType::E_PLAYER1)
			{
				rotation1 = it->FindComponent<TransformComponent>()->getRotation();
			}
			if (it->GetEntityType() == EntityType::E_PLAYER2)
			{
				rotation2 = it->FindComponent<TransformComponent>()->getRotation();
			}
		}

		//static float particleTime = 0;
//		particleTime += dt;

	//	if (particleTime > 0.6)
		//{
			// if in case game over (one of player wins than particle out)
		// still have an error now it does not show the both foot in the right order.
			if (m_particleP1Pos.size())
			{
				if (hp[0]->GetHP() <= 0 || hp[1]->GetHP() <= 0)
					return;
				if (m_rightOrLeft)
					particleType = Particle_Type::Walk_Particle_1;
				else if (!m_rightOrLeft)
					particleType = Particle_Type::Walk_Particle_2;

				m_particle->CreateParticle(m_particleP1Pos.front(), particleType, rotation1);
				m_particleP1Pos.pop_front();
			}
			if (m_particleP2Pos.size())
			{
				if (hp[0]->GetHP() <= 0 || hp[1]->GetHP() <= 0)
					return;
				m_particle->CreateParticle(m_particleP2Pos.front(), particleType, rotation2);
				m_particleP2Pos.pop_front();
			}

//			std::cout << particleNumber << std::endl;
//			particleTime = 0;
//		}

		if (m_particlePos.size())
		{
			if (hp[0]->GetHP() <= 0 || hp[1]->GetHP() <= 0)
				return;
			m_particle->CreateParticle(m_particlePos.front(), Particle_Type::Snow_Particle);
			m_particlePos.pop_front();
		}
	}

	void GameLogic::CharMovement(Entity* entity, CharacterMove* p_CM, float dt)
	{

		TransformComponent* transform = entity->FindComponent<TransformComponent>();
		RigidBodyComponent* rigid = entity->FindComponent<RigidBodyComponent>();

		if (entity->GetEntityType() == EntityType::E_PLAYER1)
		{
			if (rigid->GetKnockback())
				return;

			//if (Player1->IsConnected())
			//{
			//	if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
			//	{
			//		p_CM->ReSetSnowball();
			//		if (p_CM->IsMovable(COLLISION_FLAG::COL_UP))
			//		{
			//			glm::vec3 pos = transform->getTranslation();
			//			pos.z -= p_CM->GetSpeed() *dt;
			//			transform->setTranslation(pos);
			//			transform->setRotation(C_UP);
			//			rigid->SetDirection(S_UP);
			//			p_CM->ResetCollisionFlag();
			//		}

			//		if (!m_pAS->IsSoundPlaying(WALKING_SFX))
			//			m_pAS->Play_Sound(WALKING_SFX, false);
			//		//CreatePaticle(entity, dt);
			//	}
			//	else if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
			//	{
			//		p_CM->ReSetSnowball();
			//		if (p_CM->IsMovable(COLLISION_FLAG::COL_LEFT))
			//		{
			//			glm::vec3 pos = transform->getTranslation();
			//			pos.x -= p_CM->GetSpeed()*dt;
			//			transform->setTranslation(pos);
			//			transform->setRotation(C_LEFT);
			//			rigid->SetDirection(S_LEFT);
			//			p_CM->ResetCollisionFlag();
			//		}

			//		if (!m_pAS->IsSoundPlaying(WALKING_SFX))
			//			m_pAS->Play_Sound(WALKING_SFX, false);
			//		//CreatePaticle(entity, dt);
			//	}
			//	else if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			//	{
			//		p_CM->ReSetSnowball();
			//		if (p_CM->IsMovable(COLLISION_FLAG::COL_DOWN))
			//		{
			//			glm::vec3 pos = transform->getTranslation();
			//			pos.z += p_CM->GetSpeed()*dt;
			//			transform->setTranslation(pos);
			//			transform->setRotation(C_DOWN);
			//			rigid->SetDirection(S_DOWN);
			//			p_CM->ResetCollisionFlag();
			//		}
			//		if (!m_pAS->IsSoundPlaying(WALKING_SFX))
			//			m_pAS->Play_Sound(WALKING_SFX, false);
			//		//CreatePaticle(entity, dt);
			//	}
			//	else if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
			//	{
			//		p_CM->ReSetSnowball();
			//		if (p_CM->IsMovable(COLLISION_FLAG::COL_RIGHT))
			//		{
			//			glm::vec3 pos = transform->getTranslation();
			//			pos.x += p_CM->GetSpeed()*dt;
			//			transform->setTranslation(pos);
			//			transform->setRotation(C_RIGHT);
			//			rigid->SetDirection(S_RIGHT);
			//			p_CM->ResetCollisionFlag();
			//		}

			//		if (!m_pAS->IsSoundPlaying(WALKING_SFX))
			//			m_pAS->Play_Sound(WALKING_SFX, false);
			//		//CreatePaticle(entity, dt);
			//	}
			//}
			//else
			{
				if (m_pIS->IsPressed(VK::KEY_W))
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
				else if (m_pIS->IsPressed(VK::KEY_A))
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
				else if (m_pIS->IsPressed(VK::KEY_S))
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
				else if (m_pIS->IsPressed(VK::KEY_D))
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


		if (entity->GetEntityType() == EntityType::E_PLAYER2)
		{
			if (rigid->GetKnockback())
				return;

			//if (Player2->IsConnected())
			//{
			//	if (Player2->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
			//	{
			//		p_CM->ReSetSnowball();
			//		if (p_CM->IsMovable(COLLISION_FLAG::COL_UP))
			//		{
			//			//todo sdfsdfsdfsdf
			//			/*if (!p_CM->IsMovable(COLLISION_FLAG::COL_LEFT) && !p_CM->IsMovable(COLLISION_FLAG::COL_RIGHT))
			//			{
			//				glm::vec3 pos = transform->getTranslation();
			//				pos.z -= p_CM->GetSpeed()*dt;
			//				transform->setTranslation(pos);
			//			}*/
			//			glm::vec3 pos = transform->getTranslation();
			//			pos.z -= p_CM->GetSpeed()*dt;
			//			transform->setTranslation(pos);
			//			transform->setRotation(C_UP);
			//			rigid->SetDirection(S_UP);
			//			p_CM->ResetCollisionFlag();
			//		}
			//		if (!m_pAS->IsSoundPlaying(WALKING_SFX))
			//			m_pAS->Play_Sound(WALKING_SFX, false);
			//		//CreatePaticle(entity, dt);
			//	}
			//	else if (Player2->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
			//	{
			//		p_CM->ReSetSnowball();
			//		if (p_CM->IsMovable(COLLISION_FLAG::COL_LEFT))
			//		{
			//			glm::vec3 pos = transform->getTranslation();
			//			pos.x -= p_CM->GetSpeed()*dt;
			//			transform->setTranslation(pos);
			//			transform->setRotation(C_LEFT);
			//			rigid->SetDirection(S_LEFT);
			//			p_CM->ResetCollisionFlag();
			//		}
			//		if (!m_pAS->IsSoundPlaying(WALKING_SFX))
			//			m_pAS->Play_Sound(WALKING_SFX, false);
			//		//CreatePaticle(entity, dt);
			//	}
			//	else if (Player2->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			//	{
			//		p_CM->ReSetSnowball();
			//		if (p_CM->IsMovable(COLLISION_FLAG::COL_DOWN))
			//		{
			//			glm::vec3 pos = transform->getTranslation();
			//			pos.z += p_CM->GetSpeed()*dt;
			//			transform->setTranslation(pos);
			//			transform->setRotation(C_DOWN);
			//			rigid->SetDirection(S_DOWN);
			//			p_CM->ResetCollisionFlag();
			//		}
			//		if (!m_pAS->IsSoundPlaying(WALKING_SFX))
			//			m_pAS->Play_Sound(WALKING_SFX, false);
			//		//CreatePaticle(entity, dt);
			//	}
			//	else if (Player2->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
			//	{
			//		p_CM->ReSetSnowball();
			//		if (p_CM->IsMovable(COLLISION_FLAG::COL_RIGHT))
			//		{
			//			glm::vec3 pos = transform->getTranslation();
			//			pos.x += p_CM->GetSpeed()*dt;
			//			transform->setTranslation(pos);
			//			transform->setRotation(C_RIGHT);
			//			rigid->SetDirection(S_RIGHT);
			//			p_CM->ResetCollisionFlag();
			//		}
			//		if (!m_pAS->IsSoundPlaying(WALKING_SFX))
			//			m_pAS->Play_Sound(WALKING_SFX, false);
			//		//CreatePaticle(entity, dt);
			//	}
			//}
			//else
			{
				if (m_pIS->IsPressed(VK::KEY_ARROW_UP))
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
				else if (m_pIS->IsPressed(VK::KEY_ARROW_LEFT))
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
				else if (m_pIS->IsPressed(VK::KEY_ARROW_DOWN))
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
				else if (m_pIS->IsPressed(VK::KEY_ARROW_RIGHT))
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
		GetPlayersPos(dt);
		GetSnowballPos(dt);
		//GetSnowballPos(dt);
		CreatePaticle(dt);
	}
	void GameLogic::CreateSnowBall(Entity* entity)
	{
		static double last_time_1 = 0.0;
		static double last_time_2 = 0.0;
		double cooltime1 = 1.0;
		double cooltime2 = 1.0;
		
		TransformComponent* entity_transform = entity->FindComponent<TransformComponent>();
		RigidBodyComponent* entity_rigid = entity->FindComponent<RigidBodyComponent>();
		CharacterMove* p_CM = entity->FindComponent<CharacterMove>();
		if (entity->GetName() == "Player1")
		{
			if (m_pIS->IsTriggered(VK::KEY_LEFT_SHIFT))
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
				std::string number = std::to_string(m_snowID++);
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
			}
		}
		else if (entity->GetName() == "Player2")
		{
			if (m_pIS->IsTriggered(VK::KEY_RIGHT_SHIFT))
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
				std::string number = std::to_string(m_snowID++);
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
			}
		}
	}
	void GameLogic::ScaleSnowBall(Entity* entity, float dt)
	{
		RigidBodyComponent* rigid = entity->FindComponent<RigidBodyComponent>();
		TransformComponent* transform = entity->FindComponent<TransformComponent>();

		if (/*rigid->GetID() == 3 || */rigid->GetID() == SNOW1 || rigid->GetID() == SNOW2)
		{
			if (rigid->GetIsActive())
			{
				if (transform->getScale().x <= 13.f)
					transform->setScale(transform->getScale()*1.05f);
			}
		}
	}
	
	void GameLogic::RotateSnowBall(Entity* entity, float dt)
	{
		RigidBodyComponent* rigid = entity->FindComponent<RigidBodyComponent>();
		TransformComponent* transform = entity->FindComponent<TransformComponent>();
		
		if (/*rigid->GetID() == 3 ||*/
			rigid->GetID() == SNOW1
			|| rigid->GetID() == SNOW2
			|| rigid->GetID() == BIG_SNOW)
		{
			if (rigid->GetIsActive())
			{
				// rotate a vector by a given direction by cross product(with y-axis)
				transform->setRotation(transform->getRotation() + glm::cross(rigid->GetVelocity(), glm::vec3(0.f, -1.f, 0.f)) * dt * 0.1f);
			}
		}
	}

	void GameLogic::RotateCharacter(Entity* entity, float dt)
	{
		RigidBodyComponent* rigid = entity->FindComponent<RigidBodyComponent>();
		TransformComponent* transform = entity->FindComponent<TransformComponent>();

		static bool once = true;
		if (rigid->GetID() == 1 || rigid->GetID() == 2)
		{
			//glm::vec3 orgRot = transform->getRotation();
			if (rigid->GetKnockback())
			{
				//todo character rotation
				/*if (once)
				{
					transform->setTranslation(transform->getTranslation() + glm::vec3(0.f, -100.f, 0.f));
					once = false;
				}*/
				// rotate a vector by a given direction by cross product(with y-axis)
				//transform->setRotation(transform->getRotation() + glm::cross(rigid->GetVelocity(), glm::vec3(0.f, -1.f, 0.f)) * dt * 0.1f);
				transform->setRotation(transform->getRotation() + rigid->GetVelocity() * dt * 0.1f);
				//return;
			}
			/*else
			{
				if (!once)
				{
					transform->setTranslation(transform->getTranslation() + glm::vec3(0.f, 5.f, 0.f));
					once = true;
				}
			}*/
			//if(rigid->GetKnockback())
		}
	}

	void GameLogic::ShootSnowBall(Entity* entity, float dt)
	{
		//		RigidBodyComponent* Rigid = entity->FindComponent<RigidBodyComponent>();
		//		TransformComponent* transform = entity->FindComponent<TransformComponent>();
		//
		//		if (Rigid->GetID() == 3|| Rigid->GetID() == 4|| Rigid->GetID() == 5)
		//		{
		//			// scale part
		//			if(transform->getScale().x <= 10.f)
		//				transform->setScale(transform->getScale()*1.02f);
		////			glm::vec3 pos = transform->getTranslation();
		//	//		pos.z += 1;
		//		//	transform->setTranslation(pos);
		//		}
		/*TransformComponent* transform = entity->FindComponent<TransformComponent>();


		if (m_pIS->IsPressed(VK::KEY_SPACE))
		{
		glm::vec3 pos = transform->getTranslation();
		pos.z += 1;
		transform->setTranslation(pos);
		}

		if (m_pIS->IsPressed(VK::KEY_LEFT_CONTROL))
		{
		}*/

	}
	bool GameLogic::isOverlap(Entity* entity) const
	{
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
	/*void GameLogic::Send_Message(SystemMessage* message)
	{

	}*/
	void GameLogic::SetLevelStartTime(float time)
	{
		m_levelStartTime = time;
	}
}