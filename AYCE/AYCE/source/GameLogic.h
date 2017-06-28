/*****************************************************************************/
/*!
\file   GameLogic.h
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

namespace VK
{
	class EntitySystem;
	class InputSystem;
	class AudioSystem;
	class Entity;
	class CharacterMove;
	class StateSystem;
	class Particle;

	class GameLogic : public System
	{
	public:
		GameLogic();
		virtual ~GameLogic();

		void Init() override;
		void Update(float dt) override;
		void Shutdown() override;
	//	void ProcessMessage(Message* message) override;
		void SetLevelStartTime(float time);
		void SetItemCreateTime(float time) { m_createTime = time; }
		void ClearParticleList(void)
		{
			m_particlePos.clear();
			m_particleP1Pos.clear();
			m_particleP2Pos.clear();
		}

	private:
		void CharMovement(Entity* entity,CharacterMove* p_CM,float dt);
		void CreateSnowBall(Entity* entity);
		void ShootSnowBall(Entity* entity,float dt);
		bool isOverlap(Entity* entity) const;
		void ScaleSnowBall(Entity* entity, float dt);
		void RotateSnowBall(Entity* entity, float dt);
		void RotateCharacter(Entity* entity, float dt);
		void CreatePaticle(float dt);
		void GetPlayersPos(float dt);
		void GetSnowballPos(float dt);


		Application* m_app = Application::GetAppInstance();
		EntitySystem* m_pES;
		InputSystem* m_pIS;
		AudioSystem* m_pAS;
		StateSystem* m_pStateSystem;
		int m_snowID;
		float m_levelStartTime;
		float m_createTime;
		int total_item;
		bool m_rightOrLeft;
		

		Particle* m_particle;
		std::list<glm::vec3> m_particlePos;
		std::list<glm::vec3> m_particleP1Pos;
		std::list<glm::vec3> m_particleP2Pos;

	};
}