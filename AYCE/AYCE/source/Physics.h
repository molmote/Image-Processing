/*****************************************************************************/
/*!
\file   Physics.h
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
#pragma once
#include "precompiled.h"
#include "Geometry.h"
#include "Application.h"
#include "Entity.h"
#include "System.h"
#include "Body.h"
//#include "precompiled.h"
#include "ObjectLinkedList.h"
#include <vector>

namespace VK {

	class EntitySystem;
	class StateSystem;

	class PhysicsSystem : public System
	{
	public:
		void Init() override;
		void Shutdown() override;
		void ProcessMessage(Message* message) override;


		PhysicsSystem();
		virtual ~PhysicsSystem() {};
		void Update(float dt) override;
		virtual std::string GetName() { return "Physics"; }
//		void SendMessage(Message * m);
//		GOC * TestPoint(Vec2 testPosition);
	private:
		void IntegrateBodies(float dt);
		void DetectContacts(float dt);
		void PublishResults();
		void DebugDraw();
		void Step(float dt);
		bool DebugDrawingActive;
		float TimeAccumulation;
		CollsionDatabase Collsion;
		ContactSet Contacts;
		Application* m_app = Application::GetAppInstance();
		EntitySystem* m_pES;

	public:
		bool AdvanceStep;
		bool StepModeActive;

		//typedef std::list<Body>::iterator BodyIterator;
		//std::list<Body> Bodies;

		//typedef ObjectLinkList<Body>::iterator BodyIterator;
		//ObjectLinkList<Body> Bodies;
		
	/*	typedef std::vector<Body*>::iterator BodyIterator;
		std::vector<Body*> Bodies;*/

		//Gravity of the world
		glm::vec3 Gravity;
		//Max velocity for a physics body
		float MaxVelocity;
		float MaxVelocitySq;

		//See Resolution.cpp for use
		//Position correction tolerance
		float PenetrationEpsilon;
		//Position correction resolve percentage
		float PenetrationResolvePercentage;
		StateSystem* m_pStateSystem;

	};
}
