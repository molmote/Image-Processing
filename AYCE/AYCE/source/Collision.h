/*****************************************************************************/
/*!
\file   Collision.h
\author Hyeongjin Park
\par    email: park.hyungjin\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Collision features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once //Makes sure this header is only included once

#include "math.h"
#include "Geometry.h"
#include "EntitySystem.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"

namespace VK
{
	class Body;

	///Data for a contact between two bodies.
	///Used to resolve world collisions.
	struct BodyContact
	{
		Body* Bodies[2];
		glm::vec3 Movement[2];
		glm::vec3 ContactNormal;
		float Penetration;
		float Restitution;
		float FrictionCof;

		float SeperatingVelocity;
		float ContactImpulse;
		float CalculateSeparatingVelocity();
	};

	///Base Shape class
	class Shape
	{
	public:
		enum ShapeId
		{
			SidCircle,
			SidBox,
			SidNumberOfShapes
		};
		ShapeId Id;
		Body * body;
		Shape(ShapeId pid) : Id(pid) {};
		virtual void Draw() = 0;
		virtual bool TestPoint(glm::vec3) = 0;
	};

	///Circle shape.
	class ShapeCircle : public Shape
	{
	public:
		ShapeCircle() : Shape(SidCircle), Radius(0)
		{
		};
		float Radius;
		void Draw() override;
		bool TestPoint(glm::vec3) override;
	};

	///Axis Aligned Box Shape
	class ShapeAAB : public Shape
	{
	public:
		ShapeAAB() : Shape(SidBox) {};
		glm::vec3 aabbMin, aabbMax;
		void Draw() override;
		bool TestPoint(glm::vec3) override;
	};

	class ContactSet;
	typedef bool(*CollisionTest)(Shape*a, glm::vec3 at, Shape*b, glm::vec3 bt, ContactSet*c);
	typedef bool(*CollisionTest1)(EntityListVector a, glm::vec3 at, EntityListVector b, glm::vec3 bt, ContactSet*c);

	///The collision database provides collision detection between shape types.
	class CollsionDatabase
	{
	public:
		CollsionDatabase();
		CollisionTest CollsionRegistry[Shape::SidNumberOfShapes][Shape::SidNumberOfShapes];
		CollisionTest1 CollsionRegistry1[enumShape::SidNumberOfShapes][enumShape::SidNumberOfShapes];
		bool GenerateContacts(Shape* shapeA, glm::vec3 poistionA, Shape* shapeB, glm::vec3 poistionB, ContactSet*c) const;
		void RegisterCollsionTest(Shape::ShapeId a, Shape::ShapeId b, CollisionTest test);
		void RegisterCollsionTest1(int a, int b, CollisionTest1 test);
		bool dtboxbox(Entity* a, Entity* b);
	};
}