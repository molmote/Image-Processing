/*****************************************************************************/
/*!
\file   Physics.cpp
\author Hyeongjin Park
\par    email: park.hyungjin\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Physics features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "Physics.h"
#include "EntitySystem.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"
#include "StateSystem.h"
#include "CharacterMove.h"
#include "AudioSystem.h"
#include "InputSystem.h"

namespace VK
{

	PhysicsSystem::PhysicsSystem():
		DebugDrawingActive(false),
		TimeAccumulation(0),
		m_pES(nullptr),
		AdvanceStep(false),
		StepModeActive(false),
		MaxVelocity(0),
		MaxVelocitySq(0),
		PenetrationEpsilon(0),
		PenetrationResolvePercentage(0)
	{
		m_pStateSystem = nullptr;
	}


	void PhysicsSystem::Init()
	{
		START();

		DebugDrawingActive = false;
		TimeAccumulation = 0.0f;
		Gravity = glm::vec3(0, -400, 0);
		MaxVelocity = 1000;
		MaxVelocitySq = MaxVelocity*MaxVelocity;
		PenetrationEpsilon = 0.2f;
		PenetrationResolvePercentage = 0.8f;
		StepModeActive = false;
		AdvanceStep = false;
		m_pES = m_app->getSystem<EntitySystem>();
		m_pStateSystem = m_app->getSystem<StateSystem>();

		LOG("Initialize Physics System");
	}

	void PhysicsSystem::Update(float dt)
	{
		STATE currState = m_pStateSystem->GetCurrentState();

		if (currState != STATE_PAUSE)
		{

			const float TimeStep = 1.0f / 60.0f;

			if (!StepModeActive)
			{
				TimeAccumulation += dt;
				TimeAccumulation = glm::min(TimeAccumulation, TimeStep * 5);
				if (TimeAccumulation > TimeStep)
				{
					TimeAccumulation -= TimeStep;
					Step(TimeStep);
				}

			}
			else
			{
				TimeAccumulation = 0.0f;
				if (AdvanceStep)
				{
					Step(TimeStep);
					AdvanceStep = false;
				}
			}
		}
		/*
		if (DebugDrawingActive)
		DebugDraw();*/

	}

	void PhysicsSystem::Shutdown()
	{

	}




	void PhysicsSystem::IntegrateBodies(float dt)
	{
		EntityListVector e_list = m_pES->GetDynamicEntityList();

		for (auto it : e_list)
		{
			auto Rb = it->FindComponent<RigidBodyComponent>();
			auto Tr = it->FindComponent<TransformComponent>();


			if (Rb && Tr)
			{
				Tr->setTranslation(Tr->getTranslation() + Rb->GetVelocity()*dt);
				glm::vec3 vel = Rb->GetVelocity();
				if (glm::dot(vel, vel) > MaxVelocitySq)
				{
					glm::normalize(vel);
					vel = vel * MaxVelocity;
					Rb->SetVelocity(vel);
				}
				//Rb->SetPosition(r->GetPosition() + r->GetVelocity()*dt);
				//r->SetAcceleration()
			}
		}

/*
		for (BodyIterator it = Bodies.begin(); it != Bodies.end(); ++it)
		{
			(*it)->Integrate(dt);
		}*/
	}

	void PhysicsSystem::DetectContacts(float dt)
	{
		EntityListVector e_list = m_pES->GetDynamicEntityList();

		for (unsigned i = 0; i < e_list.size(); ++i)
		{
			auto charMove = e_list[i]->FindComponent<CharacterMove>();
			Entity* snowBall = nullptr;
			if (charMove)
			{
				snowBall = charMove->GetSnowball();
			}
			if (snowBall)
			{
				VK::Application* app = VK::Application::GetAppInstance();
				VK::InputSystem* p_inputSystem = app->getSystem<VK::InputSystem>();

				if (e_list[i]->GetEntityType() == EntityType::E_PLAYER1)
				{
					if (p_inputSystem->IsTriggered(VK::KEY_LEFT_SHIFT))
					{
						AudioSystem* p_audioSystem = app->getSystem<AudioSystem>();
						p_audioSystem->Play_Sound(SHOOT_SFX, false);

						auto RbChar = e_list[i]->FindComponent<RigidBodyComponent>();
						auto RbSnow = snowBall->FindComponent<RigidBodyComponent>();
						//auto TrA = e_list[i]->FindComponent<TransformComponent>();

						RbSnow->SetIsActive(true);
						RbSnow->SetID(COMPONENTTYPE::SNOW1);
						RbSnow->SetVelocity(RbChar->GetDirection() *= 100);
						RbSnow->SetDirection(RbChar->GetDirection());

						charMove->ReSetSnowball();
						return;
					}
				}
				else if (e_list[i]->GetEntityType() == EntityType::E_PLAYER2)
				{
					if (p_inputSystem->IsTriggered(VK::KEY_RIGHT_SHIFT))
					{
						AudioSystem* p_audioSystem = app->getSystem<AudioSystem>();
						p_audioSystem->Play_Sound(SHOOT_SFX, false);

						auto RbChar = e_list[i]->FindComponent<RigidBodyComponent>();
						auto RbSnow = snowBall->FindComponent<RigidBodyComponent>();
						//auto TrA = e_list[i]->FindComponent<TransformComponent>();

						RbSnow->SetIsActive(true);
						RbSnow->SetID(COMPONENTTYPE::SNOW2);
						RbSnow->SetVelocity(RbChar->GetDirection() *= 100);
						RbSnow->SetDirection(RbChar->GetDirection());

						charMove->ReSetSnowball();
						return;
					}
				}
			}
			else
				for (unsigned j = i+1; j < e_list.size(); ++j)
				{
			
					Collsion.dtboxbox(e_list[i], e_list[j]);
			
				}
		}

		for(auto it = e_list.begin(); it != e_list.end(); )
		{
			if (!(*it)->GetAlive())
			{
				m_pES->DeleteEntity(*it);
				it = e_list.erase(it);
			}
			else
				++it;
		}


		//BodyIterator BodyStart = Bodies.begin();
		//BodyIterator BodyLast = Bodies.end();

		////Broad phase should be added this is N^2
		//for (; BodyStart != BodyLast; ++BodyStart)
		//{
		//	BodyIterator bodyB = BodyStart;
		//	++bodyB;
		//	for (; bodyB != Bodies.end(); ++bodyB)
		//	{
		//		//Do not collide static bodies with other static bodies
		//		if (!(*BodyStart)->IsStatic || !(*bodyB)->IsStatic)
		//		{
		//			Collsion.GenerateContacts((*BodyStart)->BodyShape, (*BodyStart)->Position, (*bodyB)->BodyShape, (*bodyB)->Position, &Contacts);
		//		}
		//	}
		//}
		
	}

	void PhysicsSystem::PublishResults()
	{
		//EntityListVector e_list = m_pES->GetDynamicEntityList();

		//for (int i = 0; i < e_list.size(); ++i)
		//{

		//		Collsion.dtboxbox(e_list[i], e_list[j]);

		//}

		////Commit all physics updates
		//for (BodyIterator it = Bodies.begin(); it != Bodies.end(); ++it)
		//{
		//	(*it)->PublishResults();
		//}

		////Broadcast physics collision messages AFTER physics
		////has update the bodies
		//for (unsigned i = 0; i<Contacts.NumberOfContacts; ++i)
		//{
		//	BodyContact* contact = &Contacts.contactArray[i];
		//	MessageCollide messageCollide;
		//	messageCollide.ContactNormal = contact->ContactNormal;
		//	messageCollide.Impulse = contact->ContactImpulse;
		//	messageCollide.CollidedWith = contact->Bodies[1]->GetOwner();
		//	contact->Bodies[0]->GetOwner()->SendMessage(&messageCollide);
		//	if (contact->Bodies[1] != NULL)
		//	{
		//		messageCollide.ContactNormal = -contact->ContactNormal;
		//		messageCollide.Impulse = contact->ContactImpulse;
		//		messageCollide.CollidedWith = contact->Bodies[0]->GetOwner();
		//		contact->Bodies[1]->GetOwner()->SendMessage(&messageCollide);
		//	}
		//}
	}

	void PhysicsSystem::Step(float dt)
	{
		IntegrateBodies(dt);

//		Contacts.Reset();

		DetectContacts(dt);

//		Contacts.ResolveContacts(dt);

		//PublishResults();
	}

	void PhysicsSystem::DebugDraw()
	{
		
	}


	void PhysicsSystem::ProcessMessage(Message* message)
	{
	}

}
