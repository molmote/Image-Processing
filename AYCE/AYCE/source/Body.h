/*****************************************************************************/
/*!
\file   Body.h
\author Hyeongjin Park
\par    email: park.hyungjin\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Body features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once

//#include "precompiled.h"
#include "../source/Collision.h"


///////////////////////////////////////////////////////////////////////////////////////
///
///	\file Body.h  Define Body GameComponent
///	
///	Authors: Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////


namespace VK
{
	/*const glm::vec3 C_RIGHT = glm::vec3(-(0.17f*9.f), -(0.17f*9.f), 0.f);
	const glm::vec3 C_LEFT = glm::vec3(-(0.17f*9.f), (0.17f*9.f), 0.f);
	const glm::vec3 C_UP = glm::vec3(-(0.17f*9.f), 0, 0.f);
	const glm::vec3 C_DOWN = glm::vec3(-(0.17f*9.f), (0.17f*18.f), 0.f);*/

///Body Component provides basic point physics dynamics including mass, 
	///velocity, forces, acceleration, and collision resolution.
	///Component will modify transform component attributes every frame.

	class Body
	{
	public:
		Body();
		~Body();

		void AddForce(glm::vec3 force);
		void Integrate(float dt);
		void SetPosition(glm::vec3);
		void SetVelocity(glm::vec3);
		void PublishResults();
	//	void Knockback();

		glm::vec3 GetPosition() const;
	//	std::tuple<std::string, std::string, bool> getCollisionCheck();
	//	void setCollisionCheck(std::string a, std::string b, bool check);

		//Draw the object using the debug drawer
		//void DebugDraw();

		void Initialize();
		//virtual void Serialize(ISerializer& stream);

		//std::tuple<std::string, std::string, bool> CheckCollisionAB;
		Body * Next;
		Body * Prev;
		std::string objName;
		glm::vec3 Position;
		glm::vec3 PrevPosition;
		glm::vec3 Velocity;
		glm::vec3 Acceleration;
		glm::vec3 Dir;
		float Mass;
		float InvMass;
		float Restitution;
		float Friction;
		float Damping;
		int ID;
		float HP;
		int types;
		glm::vec3 AccumulatedForce;
		COL_OBJ isKnockback;
		bool isActive;

		//Shape used for collision with this body
		Shape * BodyShape;
		//Static object are immovable fixed objects
		bool IsStatic;
		bool CheckCollision;
	};
	class ContactSet
	{
	public:
		ContactSet() : NumberOfContacts(0)
		{
		};
		BodyContact * GetNextContact();
		void ResolveContacts(float dt);
		void Reset();
	private:
		//friend class Physics;
		static const int MaxContacts = 1024;
		BodyContact contactArray[MaxContacts];
		unsigned NumberOfContacts;
		void ResolveVelocities(float dt);
		void ResolvePositions(float dt);
	};
}