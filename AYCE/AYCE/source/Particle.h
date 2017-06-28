/*****************************************************************************/
/*!
\file   Particle.h
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
#pragma once
#include "precompiled.h"
#include "System.h"
#include "Application.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "MeshComponent.h"

namespace VK
{
	enum class Particle_Type
	{
		Particle_Type_First = 0,
		Snowball = Particle_Type_First,
		SnowFlower,
		SnowFlower1,
		Walk_Particle_1,
		Walk_Particle_2,
		Snow_Particle,
		Snowball_Collision,
		Collision_Ball_Map,
		Firework_1,
		Firework_2,
		CreateSnow,
		Particle_Type_SIZE
	};
	class EntitySystem;

	class Particle : public System
	{
	public:
		Particle();
		Particle::Particle(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc);
		virtual ~Particle();

		virtual void Init();
		virtual void Update(float dt);
		virtual void Shutdown();
		virtual void ProcessMessage(Message* message);
		void isDead();
		void CreateParticle(glm::vec3 pos, Particle_Type id);
		void CreateParticle(glm::vec3 pos, Particle_Type id, glm::vec3 rotation);
		void CreateParticle(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, Particle_Type id);
		void CreateMultipleParticle(glm::vec3 pos, Particle_Type id, int particlenumber, int maxrange, int minrange);
		void CreateMultipleParticle(glm::vec3 pos, Particle_Type id, glm::vec3 rotation, int particlenumber, int maxrange, int minrange);
		void ClearParticle();
	private:
		float m_lifespan;
		int m_snowID;
		glm::vec3 m_position;
		glm::vec3 m_velocity;
		glm::vec3 m_acceleration;
		Application* m_app = Application::GetAppInstance();
		EntitySystem* m_pES;
		//unordered_map<int, MeshComponent*> m_meshCompoents;
		//unordered_map<int, RigidBodyComponent*> m_rigidBodyComponents_;
	};
}
