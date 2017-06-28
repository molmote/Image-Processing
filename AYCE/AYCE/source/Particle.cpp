/*****************************************************************************/
/*!
\file   Particle.cpp
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
#include "Particle.h"
#include "Entity.h"
#include "EntitySystem.h"
#include "StateSystem.h"
#include "Utilities.h"

namespace VK
{
	Particle::Particle()
	{
		m_pES = nullptr;
		m_lifespan = 255;
		m_position = glm::vec3(0.f);
		m_acceleration = glm::vec3(0.f, 0.f, 0.f);
		m_velocity = glm::vec3(0.f, -30.f, 0.f);
		m_snowID = 0;
		m_pES = m_app->getSystem<EntitySystem>();
	}
	Particle::Particle(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc)
	{
		m_pES = nullptr;
		m_snowID = 0;
		m_lifespan = 255;
		m_position = pos;
		m_velocity = vel;
		m_acceleration = acc;
		m_pES = m_app->getSystem<EntitySystem>();
	}
	Particle::~Particle()
	{
	}
	void Particle::Init()
	{
		
		for(unsigned int id = 0; id < static_cast<unsigned int>(Particle_Type::Particle_Type_SIZE); ++id)
		{
			switch (static_cast<Particle_Type>(id))
			{
			case Particle_Type::Particle_Type_First:
			{		
				MeshComponent* temp = new MeshComponent;
				temp->setMesh("models/Snowball.obj");
				//m_meshCompoents[id] = temp;

				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLE);
				m_rigidBodyComponents_[id]->SetHP(rand() % 150);*/

				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLE);
				m_rigidBodyComponents_[id]->SetHP(rand() % 150);*/
				break;
			}
			case Particle_Type::SnowFlower:
			{
				MeshComponent* temp = new MeshComponent;
				temp->setMesh("models/SnowFlower.obj");
				//m_meshCompoents[id] = temp;

				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLE);
				m_rigidBodyComponents_[id]->SetHP(rand() % 150);*/


				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLE);
				m_rigidBodyComponents_[id]->SetHP(rand() % 150);*/
				break;
			}
			case Particle_Type::SnowFlower1:
			{
				MeshComponent* temp = new MeshComponent;
				temp->setMesh("models/SnowFlower1.obj");
				//m_meshCompoents[id] = temp;

				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLE);
				m_rigidBodyComponents_[id]->SetHP(rand() % 150);*/

				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLE);
				m_rigidBodyComponents_[id]->SetHP(rand() % 150);*/
				break;
			}
			case Particle_Type::Walk_Particle_1:
			{
				MeshComponent* temp = new MeshComponent;
				temp->setMesh("models/Walk_Particle_1.obj");
				//m_meshCompoents[id] = temp;
				//transform->setScale(transform->getScale()*2.f);
				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLEWALK);
				m_rigidBodyComponents_[id]->SetHP(100);*/

				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLEWALK);
				m_rigidBodyComponents_[id]->SetHP(100);*/
				break;
			}
			case Particle_Type::Walk_Particle_2:
			{
				MeshComponent* temp = new MeshComponent;
				temp->setMesh("models/Walk_Particle_2.obj");
				//m_meshCompoents[id] = temp;
				//transform->setScale(transform->getScale()*2.f);


				/*m_rigidbodycomponents_[id] = new rigidbodycomponent;
				m_rigidbodycomponents_[id]->setid(componenttype::particlewalk);
				m_rigidbodycomponents_[id]->sethp(100);*/
				/*m_rigidbodycomponents_[id] = new rigidbodycomponent;
				m_rigidbodycomponents_[id]->setid(componenttype::particlewalk);
				m_rigidbodycomponents_[id]->sethp(100);*/
				break;
			}
			case Particle_Type::Snow_Particle:
			{
				MeshComponent* temp = new MeshComponent;
				temp->setMesh("models/Snow_Particle.obj");
				//m_meshCompoents[id] = temp;
				
				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLEWALK);
				m_rigidBodyComponents_[id]->SetHP(50);*/

				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLEWALK);
				m_rigidBodyComponents_[id]->SetHP(50);*/
				break;
			}
			case Particle_Type::Snowball_Collision:
			{
				MeshComponent* temp = new MeshComponent;
				temp->setMesh("models/Snowball.obj");
				//m_meshCompoents[id] = temp;
				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLECOLLISON);
				m_rigidBodyComponents_[id]->SetHP(20);*/

				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLECOLLISON);
				m_rigidBodyComponents_[id]->SetHP(20);*/
				break;
			}
			case Particle_Type::Collision_Ball_Map:
			{
				MeshComponent* temp = new MeshComponent;
				temp->setMesh("models/Snowball.obj");
				//m_meshCompoents[id] = temp;
				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLECOLLISON);
				m_rigidBodyComponents_[id]->SetHP(20);*/

				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLECOLLISON);
				m_rigidBodyComponents_[id]->SetHP(20);*/
				break;
			}
			case Particle_Type::Firework_1:
			{
				MeshComponent* temp = new MeshComponent;
				temp->setMesh("models/FireWorks_1.obj");
				//m_meshCompoents[id] = temp;
				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLECOLLISON);
				m_rigidBodyComponents_[id]->SetHP(20);*/

				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLECOLLISON);
				m_rigidBodyComponents_[id]->SetHP(20);*/
				break;
			}
			case Particle_Type::Firework_2:
			{
				MeshComponent* temp = new MeshComponent;
				temp->setMesh("models/FireWorks_2.obj");
				//m_meshCompoents[id] = temp;
				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLECOLLISON);
				m_rigidBodyComponents_[id]->SetHP(20);*/

				/*m_rigidBodyComponents_[id] = new RigidBodyComponent;
				m_rigidBodyComponents_[id]->SetID(COMPONENTTYPE::PARTICLECOLLISON);
				m_rigidBodyComponents_[id]->SetHP(20);*/
				break;
			}
			}
		}
	}
	void Particle::CreateParticle(glm::vec3 pos, Particle_Type id)
	{
		Entity* nSnow = new Entity;
		std::string number = std::to_string(m_snowID++);
		nSnow->SetName("SnowParticle" + number);
		nSnow->SetEntityType(E_PARTICLE);
		nSnow->SetListType(E_STATIC);

		//transform
		TransformComponent* transform = new TransformComponent;
		transform->setTranslation(pos);
		transform->setScale(5.f, 5.f, 5.f);
		MeshComponent* mesh = new MeshComponent;
		RigidBodyComponent* rigid = new RigidBodyComponent;
		/*
		transform->SetOwner(nSnow);
		mesh->SetOwner(nSnow);
		rigid->SetOwner(nSnow);*/

		switch (id)
		{
		case Particle_Type::Snowball:
			mesh->setMesh("models/Snowball.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLE);
			rigid->SetHP(static_cast<float>(rand() % 150));
			break;
		case Particle_Type::SnowFlower:
			mesh->setMesh("models/SnowFlower.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLE);
			rigid->SetHP(static_cast<float>(rand() % 150));
			break;
		case Particle_Type::SnowFlower1:
			mesh->setMesh("models/SnowFlower1.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLE);
			rigid->SetHP(static_cast<float>(rand() % 150));
			break;
		case Particle_Type::Walk_Particle_1:
			mesh->setMesh("models/Walk_Particle_1.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLEWALK);
			rigid->SetHP(50);
			transform->setScale(transform->getScale()*2.f);
			break;
		case Particle_Type::Walk_Particle_2:
			mesh->setMesh("models/Walk_Particle_2.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLEWALK);
			transform->setScale(transform->getScale()*2.f);
			rigid->SetHP(50);
			break;
		case Particle_Type::Snow_Particle:
			mesh->setMesh("models/Snow_Particle.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLEWALK);
			rigid->SetHP(50);
			break;
		case Particle_Type::Snowball_Collision:
			mesh->setMesh("models/Snowball.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLECOLLISON);
			rigid->SetHP(50);
			break;
		case Particle_Type::Collision_Ball_Map:
			mesh->setMesh("models/Snowball.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLECOLLISON);
			rigid->SetHP(5);
			break;
		case Particle_Type::Firework_1:
			mesh->setMesh("models/FireWorks_1.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLEWIN);
			rigid->SetHP(50);
			break;
		case Particle_Type::Firework_2:
			mesh->setMesh("models/FireWorks_2.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLEWIN);
			rigid->SetHP(50);
			break;
		case Particle_Type::CreateSnow:
			mesh->setMesh("models/SnowCreate_.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLECREATESNOW);
			rigid->SetHP(10);
			break;
		}
		rigid->SetVelocity(m_velocity);
		rigid->SetAcceleration(m_acceleration);
		nSnow->AddComponent("Transform", transform);
		nSnow->AddComponent("MeshComponent", mesh);
		nSnow->AddComponent("RigidBody", rigid);
		m_pES->AddEntity(nSnow);

	}
	void Particle::CreateParticle(glm::vec3 pos, Particle_Type id, glm::vec3 rotation)
	{
		Entity* nSnow = new Entity;
		std::string number = std::to_string(m_snowID++);
		nSnow->SetName("SnowParticle" + number);
		nSnow->SetEntityType(E_PARTICLE);
		nSnow->SetListType(E_STATIC);

		//transform
		TransformComponent* transform = new TransformComponent;
		transform->setTranslation(pos);
		transform->setScale(5.f, 5.f, 5.f);
		transform->setRotation(rotation);
		
		/*if (!m_meshCompoents[id]->GetName().empty())
			m_meshCompoents[id] = new MeshComponent;			

		if (!m_rigidBodyComponents_[id]->GetName().empty())
			m_rigidBodyComponents_[id] = new RigidBodyComponent;*/
		
			
		MeshComponent* mesh = new MeshComponent;
		RigidBodyComponent* rigid = new RigidBodyComponent;
		
		//TODO : is owner needed?
		//transform->SetOwner(nSnow);
		//mesh->SetOwner(nSnow);
		//rigid->SetOwner(nSnow);


		switch (id)
		{
		case Particle_Type::Snowball:
			mesh->setMesh("models/Snowball.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLE);
			rigid->SetHP(static_cast<float>(rand() % 150));
			break;
		case Particle_Type::SnowFlower:
			mesh->setMesh("models/SnowFlower.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLE);
			rigid->SetHP(static_cast<float>(rand() % 150));
			break;
		case Particle_Type::SnowFlower1:
			mesh->setMesh("models/SnowFlower1.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLE);
			rigid->SetHP(static_cast<float>(rand() % 150));
			break;
		case Particle_Type::Walk_Particle_1:
			mesh->setMesh("models/Walk_Particle_1.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLEWALK);
			rigid->SetHP(50);
			transform->setScale(transform->getScale()*2.f);
			break;
		case Particle_Type::Walk_Particle_2:
			mesh->setMesh("models/Walk_Particle_2.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLEWALK);
			transform->setScale(transform->getScale()*2.f);
			rigid->SetHP(50);
			break;
		case Particle_Type::Snow_Particle:
			mesh->setMesh("models/Snow_Particle.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLEWALK);
			rigid->SetHP(50);
			break;
		case Particle_Type::Snowball_Collision:
			mesh->setMesh("models/Snowball.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLECOLLISON);
			rigid->SetHP(20);
			break;
		case Particle_Type::Collision_Ball_Map:
			mesh->setMesh("models/Snowball.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLECOLLISON);
			rigid->SetHP(5);
			break;
		case Particle_Type::Firework_1:
			mesh->setMesh("models/FireWorks_1.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLEWIN);
			rigid->SetHP(50);
			break;
		case Particle_Type::Firework_2:
			mesh->setMesh("models/FireWorks_2.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLEWIN);
			rigid->SetHP(50);
			break;
		case Particle_Type::CreateSnow:
			mesh->setMesh("models/SnowCreate_.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLECREATESNOW);
			rigid->SetHP(10);
			break;
		}
		rigid->SetVelocity(m_velocity);
		rigid->SetAcceleration(m_acceleration);
		nSnow->AddComponent("Transform", transform);
		nSnow->AddComponent("MeshComponent", mesh);
		nSnow->AddComponent("RigidBody", rigid);
		m_pES->AddEntity(nSnow);

	}
	void Particle::CreateParticle(glm::vec3 pos, glm::vec3 vel,glm::vec3 acc, Particle_Type id)
	{
		Entity* nSnow = new Entity;
		std::string number = std::to_string(m_snowID++);
		nSnow->SetName("SnowParticle" + number);
		nSnow->SetEntityType(E_PARTICLE);
		nSnow->SetListType(E_STATIC);

		//transform
		TransformComponent* transform = new TransformComponent;
		transform->setTranslation(pos);
		transform->setScale(5.f, 5.f, 5.f);
		MeshComponent* mesh = new MeshComponent;
		RigidBodyComponent* rigid = new RigidBodyComponent;
		/*
		transform->SetOwner(nSnow);
		mesh->SetOwner(nSnow);
		rigid->SetOwner(nSnow);*/

		switch (id)
		{
		case Particle_Type::Snowball:
			mesh->setMesh("models/Snowball.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLE);
			rigid->SetHP(static_cast<float>(rand() % 150));
			break;
		case Particle_Type::SnowFlower:
			mesh->setMesh("models/SnowFlower.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLE);
			rigid->SetHP(static_cast<float>(rand() % 150));
			break;
		case Particle_Type::SnowFlower1:
			mesh->setMesh("models/SnowFlower1.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLE);
			rigid->SetHP(static_cast<float>(rand() % 150));
			break;
		case Particle_Type::Walk_Particle_1:
			mesh->setMesh("models/Walk_Particle_1.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLEWALK);
			rigid->SetHP(50);
			transform->setScale(transform->getScale()*2.f);
			break;
		case Particle_Type::Walk_Particle_2:
			mesh->setMesh("models/Walk_Particle_2.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLEWALK);
			transform->setScale(transform->getScale()*2.f);
			rigid->SetHP(50);
			break;
		case Particle_Type::Snow_Particle:
			mesh->setMesh("models/Snow_Particle.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLEWALK);
			rigid->SetHP(50);
			break;
		case Particle_Type::Snowball_Collision:
			mesh->setMesh("models/Snowball.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLECOLLISON);
			rigid->SetHP(50);
			break;
		case Particle_Type::Collision_Ball_Map:
			mesh->setMesh("models/Snowball.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLECOLLISON);
			rigid->SetHP(5);
			break;
		case Particle_Type::Firework_1:
			mesh->setMesh("models/FireWorks_1.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLEWIN);
			rigid->SetHP(50);
			break;
		case Particle_Type::Firework_2:
			mesh->setMesh("models/FireWorks_2.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLEWIN);
			rigid->SetHP(50);
			break;
		case Particle_Type::CreateSnow:
			mesh->setMesh("models/SnowCreate_.obj");
			rigid->SetID(COMPONENTTYPE::PARTICLECREATESNOW );
			rigid->SetHP(10);
			break;
		}
		rigid->SetVelocity(vel);
		rigid->SetAcceleration(acc);
		nSnow->AddComponent("Transform", transform);
		nSnow->AddComponent("MeshComponent", mesh);
		nSnow->AddComponent("RigidBody", rigid);
		m_pES->AddEntity(nSnow);

	}
	void Particle::Update(float dt)
	{
		EntityListVector e_StaticList = m_pES->GetStaticEntityList();
		for (auto it : e_StaticList)
		{
			RigidBodyComponent* p_RB = it->FindComponent<RigidBodyComponent>();
			TransformComponent* p_TS = it->FindComponent<TransformComponent>();
			if (p_RB != nullptr)
			{
				switch (p_RB->GetID())
				{
				case COMPONENTTYPE::PARTICLE:
					p_RB->SetHP(p_RB->GetHP() - 0.3f);
					m_velocity = glm::vec3(rand() % 20 - 20, -30, rand() % 20 - 20);
					p_TS->setTranslation(p_TS->getTranslation() + m_velocity * dt);
					if (p_RB->GetHP() <= 0)
					{
						p_RB->SetHP(static_cast<float>(rand() % 100 + 100));
						int v1 = rand() % 1000 - 800;
						int v2 = rand() % 500;
						int v3 = rand() % 1000 - 600;
						p_TS->setTranslation(glm::vec3(v1, v2, v3));
					}
					break;
				case COMPONENTTYPE::PARTICLEWALK:
					p_RB->SetHP(p_RB->GetHP() - 1);
					//m_velocity = glm::vec3(rand() % 20 - 10, rand() % 20 - 10, rand() % 20 - 10);
					//p_TS->setTranslation(p_TS->getTranslation() + m_velocity * dt);

					//p_TS->setRotation(glm::vec3(0,0.5f,0));
					if (p_RB->GetHP() <= 0)
						m_pES->DeleteEntity(it);
					break;
				case COMPONENTTYPE::PARTICLECOLLISON:
					p_RB->SetHP(p_RB->GetHP() - 1);

					p_TS->setTranslation(p_TS->getTranslation() + p_RB->GetVelocity() * dt);
					//p_TS->setRotation(glm::vec3(0,0.5f,0));

					if (p_RB->GetHP() <= 0)
						m_pES->DeleteEntity(it);
					break;
				case COMPONENTTYPE::PARTICLEWIN:
					p_RB->SetHP(p_RB->GetHP() - 1);
					p_TS->setTranslation(p_TS->getTranslation() + p_RB->GetVelocity()*dt);

					if (p_RB->GetHP() <= 0)
					{
						p_RB->SetID(COMPONENTTYPE::PARTICLEFIREWORKS);
						int v1 = rand() % 200 - 100;//
						int v2 = rand() % 200 - 100;
						int v3 = rand() % 200 - 100;
						p_RB->SetHP(500);
						p_RB->SetVelocity(glm::vec3(v1, v2, v3));
					}
					break;
				case COMPONENTTYPE::PARTICLEFIREWORKS:
					p_RB->SetHP(p_RB->GetHP() - 1);
					p_TS->setTranslation(p_TS->getTranslation() + p_RB->GetVelocity()*dt);
					if (p_RB->GetHP() <= 0)
						m_pES->DeleteEntity(it);
					break;
				case COMPONENTTYPE::PARTICLECREATESNOW:
					p_RB->SetHP(p_RB->GetHP() - 1);
					p_TS->setTranslation(p_TS->getTranslation() + glm::vec3(0,-20,0)*dt);
					if (p_RB->GetHP() <= 0)
						m_pES->DeleteEntity(it);
					break;
				}
			}
		}
	}
	void Particle::Shutdown()
	{

	}
	void Particle::ProcessMessage(Message* message)
	{

	}
	void Particle::isDead()
	{
		EntityListVector e_DynamicList = m_pES->GetDynamicEntityList();
		for (auto it : e_DynamicList)
		{
			RigidBodyComponent* p_RB = it->FindComponent<RigidBodyComponent>();

			if (p_RB->GetID() == COMPONENTTYPE::PARTICLE)
				if (p_RB->GetHP() < 0)
					m_pES->DeleteEntity(it);
		}
	}
	void Particle::CreateMultipleParticle(glm::vec3 pos, Particle_Type id, int particlenumber, int maxrange, int minrange)
	{
		int max = maxrange - minrange;

		for (int i = 0; i < particlenumber; ++i)
		{
			glm::vec3 randomPos = glm::vec3(Utilities::RandMinMax(minrange, maxrange), Utilities::RandMinMax(minrange, maxrange), Utilities::RandMinMax(minrange, maxrange));
			CreateParticle(pos + randomPos, id);
		}			
	}
	void Particle::CreateMultipleParticle(glm::vec3 pos, Particle_Type id, glm::vec3 rotation, int particlenumber, int maxrange, int minrange)
	{
		for (int i = 0; i < particlenumber; ++i)
		{
			glm::vec3 randomPos = glm::vec3(Utilities::RandMinMax(minrange, maxrange), Utilities::RandMinMax(minrange, maxrange), Utilities::RandMinMax(minrange, maxrange));
			CreateParticle(pos + randomPos, id, rotation);
		}
	}
	void Particle::ClearParticle()
	{
		EntityListVector e_StaticList = m_pES->GetStaticEntityList();

		for (auto it = e_StaticList.begin(); it != e_StaticList.end(); )
		{
			if ((*it)->GetEntityType() == E_PARTICLE)
			{
				m_pES->DeleteEntity(*it);
				it = e_StaticList.erase(it);
			}
			else
				it++;
		}
	}
}