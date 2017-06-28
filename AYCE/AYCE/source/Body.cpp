/*****************************************************************************/
/*!
\file   Body.cpp
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

#include "body.h"
#include "Physics.h"

namespace VK
{
	///Body Component provides basic point physics dynamics including mass, 
	///velocity, forces, acceleration, and collision resolution.
	///Component will modify transform component attributes every frame.

	Body::Body(): Next(nullptr), Prev(nullptr)
	{
		Position = glm::vec3(0, 0, 0);
		PrevPosition = glm::vec3(0, 0, 0);
		Velocity = glm::vec3(0, 0, 0);
		Mass = 1.0f;
		InvMass = 0.0f;
		Damping = 1.0f;
		Acceleration = glm::vec3(0, 0, 0);
		BodyShape = nullptr;
		Friction = 0.0f;
		Restitution = 0.0f;
		IsStatic = true;
		CheckCollision = false;
		AccumulatedForce = glm::vec3(0, 0, 0);
		ID = 0;
		objName = "";
		HP = 0;
		types = COMPONENTTYPE::DEFAULT;
		//CheckCollisionAB = std::tuple<std::string, std::string, bool>("", "", false);
		isKnockback = COL_OBJ::COL_NONE;
		Dir = glm::vec3(0, 0, 0);
		isActive = false;
	}

	Body::~Body()
	{
		delete BodyShape;
	}
	//void Body::setCollisionCheck(std::string a, std::string b, bool check)
	//{
	//	CheckCollision = check;
	//	CheckCollisionAB = std::tuple<std::string, std::string, bool>(a, b, check);
	//	//std::tuple<std::string, std::string, bool>(a, b, check)
	//}
	//std::tuple<std::string, std::string, bool> Body::getCollisionCheck()
	//{
	//	return CheckCollisionAB;
	//}
	void Body::AddForce(glm::vec3 force)
	{
		AccumulatedForce += force;
	}
	void Body::Integrate(float dt)
	{
		//Do not integrate static bodies
		if (IsStatic) return;

		//Knockback();

		//Store prev position
		PrevPosition = Position;

		//Integrate the position using Euler 
		Position = Position + Velocity * dt; //acceleration term is small

											 //Determine the acceleration

		Acceleration = glm::vec3(0.0,0.0,0.0);
		glm::vec3 newAcceleration = AccumulatedForce * InvMass + Acceleration;

		//Integrate the velocity
		Velocity = Velocity + newAcceleration * dt;

		//Dampen the velocity for numerical stability and soft drag
		Velocity *= std::pow(Damping, dt);

		//Clamp to velocity max for numerical stability
		if (glm::dot(Velocity, Velocity) > 1000000)
		{
			glm::normalize(Velocity);
			Velocity *= 1000;
		}
		//Clear the force
		AccumulatedForce = glm::vec3(0, 0, 0);
	}

	void Body::SetPosition(glm::vec3 p)
	{
		Position = p;
	
	}
	glm::vec3 Body::GetPosition() const
	{
		return Position;
	}
	void Body::SetVelocity(glm::vec3 v)
	{
		Velocity = v;
	}
	////Draw the object using the debug drawer
	//void DebugDraw();

	void Body::Initialize()
	{
		//Get the transform to write results to
		//tx; //GetOwner()->has(Transform);

		//GetOwner()->

		//Get the starting position
		//Add this body to the body list
//		PHYSICS->Bodies.push_back(this);
		//PHYSICS->body.push_back(this);

		//If mass is zero object is interpreted
		//to be static
		if (Mass > 0.0f)
		{
			IsStatic = false;
			InvMass = 1.0f / Mass;
		}
		else
		{
			IsStatic = true;
			InvMass = 0.0f;
		}

		BodyShape->body = this;
	}
	BodyContact * ContactSet::GetNextContact()
	{
		//ErrorIf(NumberOfContacts == MaxContacts, "Maximum number of contacts reached. There is too many colliding objects in the simulation.");
		return &contactArray[NumberOfContacts++];
	}

	void ContactSet::Reset()
	{
		NumberOfContacts = 0;
	}

	float BodyContact::CalculateSeparatingVelocity()
	{
		glm::vec3 relativeVelocity = Bodies[0]->Velocity - Bodies[1]->Velocity;

		//Get the separating velocity by projecting along the contact normal
		SeperatingVelocity = glm::dot(relativeVelocity, ContactNormal);

		return SeperatingVelocity;
	}

	void Body::PublishResults()
	{
		//getTransform().setTranslation(Position);
	}

	void ResloveContactVelocity(BodyContact& c, float dt)
	{
		//Find the velocity of the two object along the contact normal
		float separatingVelocity = c.CalculateSeparatingVelocity();

		if (separatingVelocity > 0)
		{
			//The objects are no longer moving towards each other
			//or the contact they are stationary
			c.ContactImpulse = 0;
			return;
		}

		//This velocity of this collision has to be resolved.

		//To model inelastic collision the kinetic energy of the bodies 
		//needs to be reduced.
		//This technique uses the combined restitution to determine what percentage
		//of the energy along the collision normal is conserved
		float newSepVelocity = -separatingVelocity * c.Restitution;

		const bool AccelerationBuildUp = true;
		//When an object is resting on the ground it is constantly falling
		//due to gravity. This acceleration need be removed or objects will
		//jitter on the ground.

		if (AccelerationBuildUp)
		{
			// Check the velocity build-up due to acceleration only
			glm::vec3 accCausedVelocity = c.Bodies[0]->Acceleration - c.Bodies[1]->Acceleration;
			float accCausedSepVelocity = glm::dot(accCausedVelocity, c.ContactNormal) * dt;

			// If we've got a closing velocity due to acceleration build-up,
			// remove it from the new separating velocity
			if (accCausedSepVelocity < 0)
			{
				newSepVelocity += c.Restitution * accCausedSepVelocity;

				// Make sure we haven't removed more than was
				// there to remove.
				if (newSepVelocity < 0) newSepVelocity = 0;
			}
		}

		//What is the total change in velocity for the contact?
		float deltaVelocity = newSepVelocity - separatingVelocity;

		//The delta velocity is applied to each object proportional to inverse
		//mass. So the more massive an object is the less of the change
		//in velocity it will receive.
		float totalInverseMass = c.Bodies[0]->InvMass + c.Bodies[1]->InvMass;

		// Calculate the impulse to apply
		float impulse = deltaVelocity / totalInverseMass;

		c.ContactImpulse = impulse;

		// Find the amount of impulse per unit of inverse mass
		glm::vec3 impulsePerIMass = c.ContactNormal * impulse;

		// Apply impulses: they are applied in the direction of the contact,
		// and in proportion to inverse mass.
		c.Bodies[0]->Velocity = c.Bodies[0]->Velocity + impulsePerIMass * c.Bodies[0]->InvMass;
		// The other body goes in the opposite direction
		c.Bodies[1]->Velocity = c.Bodies[1]->Velocity + impulsePerIMass * -c.Bodies[1]->InvMass;
	}

	void ResolvePenetration(BodyContact& c, float dt)
	{
		// The movement of each object is based on their inverse mass, so
		// total that.
		float totalInverseMass = c.Bodies[0]->InvMass + c.Bodies[1]->InvMass;

		// Find the amount of penetration resolution per unit of inverse mass
		glm::vec3 movePerIMass = c.ContactNormal * (c.Penetration / totalInverseMass);

		//If stack stability can be increased by not resolving all the penetrations
		//in one step
		movePerIMass *= 0.8;

		// Calculate the the movement amounts
		c.Movement[0] = movePerIMass * c.Bodies[0]->InvMass;
		c.Movement[1] = movePerIMass * -c.Bodies[1]->InvMass;

		// Apply the penetration resolution
		c.Bodies[0]->Position = c.Bodies[0]->Position + c.Movement[0];
		c.Bodies[1]->Position = c.Bodies[1]->Position + c.Movement[1];
	}


	//Resolve Positions
	void ContactSet::ResolvePositions(float dt)
	{
		//Resolving penetrations is a tricky problem with large stacks. The only real way to get 
		//good results is by considering all contacts at once at solving them together. This method basically 
		//just keeps poking the bodies until the penetrations are below the penetration slop epsilon. 
		//Better solutions involve building contact graphs, shock propagation and constraint based
		//solvers.
		unsigned iterationsRun = 0;
		unsigned maxIterations = NumberOfContacts * 5;
		const float positionEpsilon = 0.003f;

		while (iterationsRun < maxIterations)
		{
			// Find biggest penetration greater than
			//the correction epsilon
			float maxPenetration = positionEpsilon;
			unsigned contactIndex = NumberOfContacts;
			for (unsigned i = 0; i<NumberOfContacts; i++)
			{
				if (contactArray[i].Penetration > maxPenetration)
				{
					maxPenetration = contactArray[i].Penetration;
					contactIndex = i;
				}
			}
			if (contactIndex == NumberOfContacts) break;

			//Resolve the penetration
			ResolvePenetration(contactArray[contactIndex], dt);

			// Update the penetrations for all related contacts
			glm::vec3 * movement = contactArray[contactIndex].Movement;
			for (unsigned i = 0; i < NumberOfContacts; i++)
			{
				if (contactArray[i].Bodies[0] == contactArray[contactIndex].Bodies[0])
					contactArray[i].Penetration -= glm::dot(movement[0], contactArray[i].ContactNormal);
				else if (contactArray[i].Bodies[0] == contactArray[contactIndex].Bodies[1])
					contactArray[i].Penetration -= glm::dot(movement[1], contactArray[i].ContactNormal);
				if (contactArray[i].Bodies[1])
				{
					if (contactArray[i].Bodies[1] == contactArray[contactIndex].Bodies[0])
						contactArray[i].Penetration += glm::dot(movement[0], contactArray[i].ContactNormal);
					else if (contactArray[i].Bodies[1] == contactArray[contactIndex].Bodies[1])
						contactArray[i].Penetration += glm::dot(movement[1], contactArray[i].ContactNormal);
				}
			}
			++iterationsRun;
		}
	}

	//Resolve Velocities of all contacts
	void ContactSet::ResolveVelocities(float dt)
	{
		unsigned iterationsRun = 0;
		unsigned maxIterations = NumberOfContacts * 5;
		while (iterationsRun < maxIterations)
		{
			// Find the contact with the largest closing velocity;
			float maxVelocity = FLT_MAX;
			unsigned contactIndex = NumberOfContacts;
			for (unsigned i = 0; i < NumberOfContacts; i++)
			{
				float sepVel = contactArray[i].CalculateSeparatingVelocity();
				if (sepVel < 0 && sepVel < maxVelocity)
				{
					maxVelocity = sepVel;
					contactIndex = i;
				}
			}

			// Do we have anything worth resolving?
			if (contactIndex == NumberOfContacts) break;

			// Resolve this contact velocity
			ResloveContactVelocity(contactArray[contactIndex], dt);

			++iterationsRun;
		}
	}

	void ContactSet::ResolveContacts(float dt)
	{
		this->ResolvePositions(dt);
		this->ResolveVelocities(dt);
	}

}