/*****************************************************************************/
/*!
\file   Collision.cpp
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

#include "Collision.h"
#include "Physics.h"
#include "Body.h"
#include "InputSystem.h"
#include "Application.h"
#include "CharacterMove.h"
#include "CharacterHP.h"
#include "AudioSystem.h"
#include "ItemUsedComponent.h"
#include "SnowballMove.h"
#include "HoleComponent.h"
#include "Particle.h"
#include "Factory.h"
#include "AudioSystem.h"

namespace VK
{

	//Functions that determine how restitution and friction 
	//are determined between body pairs.

	//There really is no physically accurate way to mathematically combine 
	//these factors. A material does not have a single friction or restitution.
	//The physically accurate way is to have a large database that
	//define the relationship between all physical materials or simulate 
	//molecular physics.

	float DetermineRestitution(Body * a, Body * b)
	{
		return glm::min(a->Restitution, b->Restitution);
	}

	float DetermineFriction(Body * a, Body * b)
	{
		return glm::sqrt(a->Friction*b->Friction);
	}

	void ShapeCircle::Draw()
	{
		//Drawer::Instance.DrawCircle(body->Position, Radius);
	}

	bool ShapeCircle::TestPoint(glm::vec3 testPoint)
	{
		glm::vec3 delta = body->Position - testPoint;
		float dis = glm::length(delta);
		if (dis < Radius)
			return true;
		else
			return false;
	}


	void ShapeAAB::Draw()
	{
		//Drawer::Instance.MoveTo(body->Position + Vec2(Extents.x, Extents.y));
		//Drawer::Instance.LineTo(body->Position + Vec2(-Extents.x, Extents.y));
		//Drawer::Instance.LineTo(body->Position + Vec2(-Extents.x, -Extents.y));
		//Drawer::Instance.LineTo(body->Position + Vec2(Extents.x, -Extents.y));
		//Drawer::Instance.LineTo(body->Position + Vec2(Extents.x, Extents.y));
		//Drawer::Instance.Flush();
	}

	bool ShapeAAB::TestPoint(glm::vec3 testPoint)
	{
		/*glm::vec3 delta = body->Position - testPoint;
		if (fabs(delta.x) < Extents.x)
		{
			if (fabs(delta.y) < Extents.y)
			{
				return true;
			}
		}*/
		return false;
	}

	/////////////////////Collsion Detection Functions////////////////////

	bool DetectCollisionCircleCircle(Shape*a, glm::vec3 at, Shape*b, glm::vec3 bt, ContactSet*c)
	{
		ShapeCircle * spA = static_cast<ShapeCircle*>(a);
		ShapeCircle * spB = static_cast<ShapeCircle*>(b);

		if (Geometry::SphereSphere(at, spA->Radius, bt, spA->Radius))
			return true;

		return false;
	}

	bool  DetectCollisionCircleAABox(Shape*a, glm::vec3 at, Shape*b, glm::vec3 bt, ContactSet*c)
	{
		//ShapeCircle * spA = static_cast<ShapeCircle*>(a);
		//ShapeAAB * boxB = static_cast<ShapeAAB*>(b);



		return false;
	}

	bool  DetectCollisionAABoxAABox(Shape*a, glm::vec3 at, Shape*b, glm::vec3 bt, ContactSet*c)
	{
		VK::Application* app = VK::Application::GetAppInstance();
		VK::InputSystem* p_inputSystem = app->getSystem<VK::InputSystem>();

		ShapeAAB * boxA = static_cast<ShapeAAB*>(a);
		ShapeAAB * boxB = static_cast<ShapeAAB*>(b);
		//bool returnvalue = Geometry::AabbAabb(boxA->aabbMin + at, boxA->aabbMax + at, boxB->aabbMin + bt, boxB->aabbMax + bt);
		
		if(a->body->types != b->body->types)
		{
			if (Geometry::AabbAabb(boxA->aabbMin + at, boxA->aabbMax + at, boxB->aabbMin + bt, boxB->aabbMax + bt))
			{
				//	std::cout << "position a : " << "x : " << a->body->Position.x << "\t y : " << a->body->Position.y << "\t z : " << a->body->Position.z << std::endl;
				//	std::cout << "position b : " << "x : " << b->body->Position.x << "\t y : " << b->body->Position.y << "\t z : " << b->body->Position.z << std::endl;
				std::cout << "Id : " << a->body->objName << "," << b->body->objName << " - " << "collision true" << std::endl;
				//	std::cout << "a CheckCollision : " << a->body->CheckCollision << ", b CheckCollision : " << b->body->CheckCollision<<std::endl;
				//a->body->CheckCollision = true;
				//b->body->CheckCollision = true;
				//a->body->setCollisionCheck(a->body->objName, b->body->objName, true);
				//b->body->setCollisionCheck(b->body->objName, a->body->objName, true);

				// press space to activate balls
				if (p_inputSystem->IsTriggered(VK::KEY_RIGHT_CONTROL))
				{
					if (a->body->types == COMPONENTTYPE::SNOW && b->body->types == COMPONENTTYPE::CHARACTER2)
					{
						a->body->isActive = true;
						a->body->types = COMPONENTTYPE::SNOW2;
						a->body->Velocity = b->body->Dir;
						a->body->Velocity *= 100;
					}
					if (b->body->types == COMPONENTTYPE::SNOW && a->body->types == COMPONENTTYPE::CHARACTER2)
					{
						b->body->isActive = true;
						b->body->types = COMPONENTTYPE::SNOW2;
						b->body->Velocity = a->body->Dir;
						b->body->Velocity *= 100;
					}
				}
				if (p_inputSystem->IsPressed(VK::KEY_LEFT_ALT))
				{

					if (a->body->types == COMPONENTTYPE::SNOW && b->body->types == COMPONENTTYPE::CHARACTER1)
					{
						a->body->isActive = true;
						a->body->types = COMPONENTTYPE::SNOW1;
						a->body->Velocity = b->body->Dir;
						a->body->Velocity *= 100;
					}
					if (b->body->types == COMPONENTTYPE::SNOW && a->body->types == COMPONENTTYPE::CHARACTER1)
					{
						b->body->isActive = true;
						b->body->types = COMPONENTTYPE::SNOW1;
						b->body->Velocity = a->body->Dir;
						b->body->Velocity *= 100;
					}
				}
				// check which character build which ball

				if (b->body->types == COMPONENTTYPE::CHARACTER2 && a->body->types == COMPONENTTYPE::BIG_SNOW)
				{
					b->body->HP -= 0.5;
					b->body->isKnockback = COL_BIG_SNOW;
					//b->body->SetPosition(b->body->GetPosition() + a->body->Velocity);
					glm::vec3 newvel = a->body->Dir;
					newvel *= 8;
					b->body->SetVelocity(newvel);
				}
				if (b->body->types == COMPONENTTYPE::CHARACTER1 && a->body->types == COMPONENTTYPE::BIG_SNOW)
				{
					b->body->HP -= 0.5;
					b->body->isKnockback = COL_BIG_SNOW;
					//b->body->SetPosition(b->body->GetPosition() + a->body->Velocity);
					glm::vec3 newvel = a->body->Dir;
					newvel *= 8;
					b->body->SetVelocity(newvel);
				}
				if (a->body->types == COMPONENTTYPE::CHARACTER1 && b->body->types == COMPONENTTYPE::BIG_SNOW)
				{
					a->body->HP -= 0.5;
					a->body->isKnockback = COL_BIG_SNOW;
					//a->body->SetPosition(a->body->GetPosition() + b->body->Velocity);
					glm::vec3 newvel = b->body->Dir;
					newvel *= 8;
					a->body->SetVelocity(newvel);
				}
				if (a->body->types == COMPONENTTYPE::CHARACTER2 && b->body->types == COMPONENTTYPE::BIG_SNOW)
				{
					a->body->HP -= 0.5;
					a->body->isKnockback = COL_BIG_SNOW;
					a->body->SetPosition(a->body->GetPosition() + b->body->Velocity);
					/*glm::vec3 newvel = b->body->Dir;
					newvel *= 8;
					a->body->SetVelocity(newvel);*/
				}
				if (a->body->types != COMPONENTTYPE::BIG_SNOW)
				{
					if (b->body->types == COMPONENTTYPE::CHARACTER2 && a->body->types == COMPONENTTYPE::SNOW1)
					{
						b->body->HP -= 0.5;
						
						b->body->isKnockback = COL_SMALL_SNOW;
						//b->body->SetPosition(b->body->GetPosition() + a->body->Velocity);
						glm::vec3 newvel = a->body->Dir;
						newvel *= 8;
						b->body->SetVelocity(newvel);
					}
					if (b->body->types == COMPONENTTYPE::CHARACTER1 && a->body->types == COMPONENTTYPE::SNOW2)
					{
						b->body->HP -= 0.5;
						b->body->isKnockback = COL_SMALL_SNOW;
						//b->body->SetPosition(b->body->GetPosition() + a->body->Velocity);
						glm::vec3 newvel = a->body->Dir;
						newvel *= 8;
						b->body->SetVelocity(newvel);
					}
				}
				if (b->body->types != COMPONENTTYPE::BIG_SNOW)
				{
					if (a->body->types == COMPONENTTYPE::CHARACTER1 && b->body->types == COMPONENTTYPE::SNOW2)
					{
						a->body->HP -= 0.5;
						a->body->isKnockback = COL_SMALL_SNOW;
						//a->body->SetPosition(a->body->GetPosition() + b->body->Velocity);
						glm::vec3 newvel = b->body->Dir;
						newvel *= 8;
						a->body->SetVelocity(newvel);
					}
					if (a->body->types == COMPONENTTYPE::CHARACTER2 && b->body->types == COMPONENTTYPE::SNOW1)
					{
						a->body->HP -= 0.5;
						a->body->isKnockback = COL_SMALL_SNOW;
						//a->body->SetPosition(a->body->GetPosition() + b->body->Velocity);
						glm::vec3 newvel = b->body->Dir;
						newvel *= 8;
						a->body->SetVelocity(newvel);
					}
				}
				

				return true;
			}
		}
		a->body->isKnockback = COL_NONE;
		b->body->isKnockback = COL_NONE;

		//	a->body->setCollisionCheck(a->body->objName, b->body->objName, false);
		//	b->body->setCollisionCheck(b->body->objName, a->body->objName, false);


			//std::cout << "\t\t\t\t." << std::endl;
		return false;
	}


//Auxiliary
bool  DetectCollisionBoxCircle(Shape*a, glm::vec3 at, Shape*b, glm::vec3 bt, ContactSet*c)
{
	return DetectCollisionCircleAABox(b, bt, a, at, c);
}

bool DetectCollisionBoxBox(EntityListVector a, EntityListVector b, ContactSet*c)
{
	return false;
}

CollsionDatabase::CollsionDatabase()
{
	//Register collision tests for all the shape types
	//typedef bool(*CollisionTest)(EntityListVector *a, glm::vec3 at, EntityListVector *b, glm::vec3 bt, ContactSet*c);
	//RegisterCollsionTest()
	RegisterCollsionTest(Shape::SidCircle, Shape::SidCircle, DetectCollisionCircleCircle);
	RegisterCollsionTest(Shape::SidBox, Shape::SidBox, DetectCollisionAABoxAABox);
	RegisterCollsionTest(Shape::SidCircle, Shape::SidBox, DetectCollisionCircleAABox);
	RegisterCollsionTest(Shape::SidBox, Shape::SidCircle, DetectCollisionBoxCircle);
	//		RegisterCollsionTest1(1, 1, DetectCollisionBoxBox);
}

void CollsionDatabase::RegisterCollsionTest1(int a, int b, CollisionTest1 test)
{
	CollsionRegistry1[a][b] = test;
}

bool CollsionDatabase::dtboxbox(Entity* a, Entity* b)
{
	VK::Application* app = VK::Application::GetAppInstance();
	VK::InputSystem* p_inputSystem = app->getSystem<VK::InputSystem>();
	VK::EntitySystem* m_pES = app->getSystem<VK::EntitySystem>();
	VK::AudioSystem* m_pAS = app->getSystem<VK::AudioSystem>();


	auto RbA = a->FindComponent<RigidBodyComponent>();
	auto TrA = a->FindComponent<TransformComponent>();
	auto RbB = b->FindComponent<RigidBodyComponent>();
	auto TrB = b->FindComponent<TransformComponent>();
	auto ItemUsed_a = a->FindComponent<ItemUsedComponent>();
	auto ItemUsed_b = b->FindComponent<ItemUsedComponent>();
	auto CM_a = a->FindComponent<CharacterMove>();
	auto CM_b = b->FindComponent<CharacterMove>();
	auto Hole_a = a->FindComponent<HoleComponent>();
	auto Hole_b = b->FindComponent<HoleComponent>();
	auto SM_a = a->FindComponent<SnowballMove>();
	auto SM_b = b->FindComponent<SnowballMove>();

	// Checking Hole and Character
	if (CM_a && Hole_b)
	{
		glm::vec3 HolePos = TrB->getTranslation();
		glm::vec2 HoleRange = Hole_b->GetHoleSize()/2.f;
		glm::vec3 CharPos = TrA->getTranslation();
		//glm::vec2 CharRange_p = glm::vec2(RbA->GetAAbbMax().x, RbA->GetAAbbMax().z);
		//glm::vec2 CharRange_n = glm::vec2(RbA->GetAAbbMin().x, RbA->GetAAbbMin().z);

		if (HolePos.x + HoleRange.x > CharPos.x && HolePos.x - HoleRange.x < CharPos.x )//x range check
			if (HolePos.z + HoleRange.y > CharPos.z  && HolePos.z - HoleRange.y < CharPos.z )//z range check
				CM_a->SetFallState(true);

	}
	else if (CM_b && Hole_a)
	{
		glm::vec3 HolePos = TrA->getTranslation();
		glm::vec2 HoleRange = Hole_a->GetHoleSize() / 2.f;
		glm::vec3 CharPos = TrB->getTranslation();
		//glm::vec2 CharRange_p = glm::vec2(RbB->GetAAbbMax().x, RbB->GetAAbbMax().z);
		//glm::vec2 CharRange_n = glm::vec2(RbB->GetAAbbMin().x, RbB->GetAAbbMin().z);

		if (HolePos.x + HoleRange.x >CharPos.x  && HolePos.x - HoleRange.x < CharPos.x)//x range check
			if (HolePos.z + HoleRange.y > CharPos.z && HolePos.z - HoleRange.y < CharPos.z )//z range check
				CM_b->SetFallState(true);
	}

	if (SM_a && Hole_b)
	{
		glm::vec3 HolePos = TrB->getTranslation();
		glm::vec2 HoleRange = Hole_b->GetHoleSize() / 2.f;
		glm::vec3 SnowPos = TrA->getTranslation();
		//glm::vec2 CharRange_p = glm::vec2(RbA->GetAAbbMax().x, RbA->GetAAbbMax().z);
		//glm::vec2 CharRange_n = glm::vec2(RbA->GetAAbbMin().x, RbA->GetAAbbMin().z);

		if (HolePos.x + HoleRange.x > SnowPos.x + RbA->GetAAbbMax().x && HolePos.x - HoleRange.x < SnowPos.x + RbA->GetAAbbMin().x)//x range check
			if (HolePos.z + HoleRange.y > SnowPos.z + RbA->GetAAbbMax().z && HolePos.z - HoleRange.y < SnowPos.z + RbA->GetAAbbMin().z)//z range check
				SM_a->SetFallState(true);
	}
	else if (SM_b && Hole_a)
	{
		glm::vec3 HolePos = TrA->getTranslation();
		glm::vec2 HoleRange = Hole_a->GetHoleSize() / 2.f;
		glm::vec3 SnowPos = TrB->getTranslation();
		//glm::vec2 CharRange_p = glm::vec2(RbB->GetAAbbMax().x, RbB->GetAAbbMax().z);
		//glm::vec2 CharRange_n = glm::vec2(RbB->GetAAbbMin().x, RbB->GetAAbbMin().z);

		if (HolePos.x + HoleRange.x >SnowPos.x + RbB->GetAAbbMax().x && HolePos.x - HoleRange.x < SnowPos.x + RbB->GetAAbbMin().x)//x range check
			if (HolePos.z + HoleRange.y > SnowPos.z + RbB->GetAAbbMax().z && HolePos.z - HoleRange.y < SnowPos.z + RbB->GetAAbbMin().z)//z range check
				SM_b->SetFallState(true);
	}

	if (RbA && RbB)
	{

		if (RbA->GetID() != RbB->GetID())
		{
			if (Geometry::AabbAabb(RbA->GetAAbbMin() + TrA->getTranslation(), RbA->GetAAbbMax() + TrA->getTranslation(), RbB->GetAAbbMin() + TrB->getTranslation(), RbB->GetAAbbMax() + TrB->getTranslation()))
			{
				//system("cls");
			//	std::cout << a->GetName() << "  " << b->GetName()<<std::endl;

				// to check overlap between player1 and 2
				if (a->GetEntityType() == EntityType::E_PLAYER1 &&
					b->GetEntityType() == EntityType::E_PLAYER2)
				{

					CharacterMove* CM1 = a->FindComponent<CharacterMove>();
					CharacterMove* CM2 = b->FindComponent<CharacterMove>();

					COLLISION_FLAG flag1;
					COLLISION_FLAG flag2;


					glm::vec3 checkpoint1 = RbA->GetDirection();
					if (checkpoint1 == S_UP)
						flag1 = COLLISION_FLAG::COL_UP;
					else if (checkpoint1 == S_RIGHT)
						flag1 = COLLISION_FLAG::COL_RIGHT;
					else if (checkpoint1 == S_DOWN)
						flag1 = COLLISION_FLAG::COL_DOWN;
					else if (checkpoint1 == S_LEFT)
						flag1 = COLLISION_FLAG::COL_LEFT;
					checkpoint1 *= 15;


					glm::vec3 checkpoint2 = RbB->GetDirection();
					if (checkpoint2 == S_UP)
						flag2 = COLLISION_FLAG::COL_UP;
					else if (checkpoint2 == S_RIGHT)
						flag2 = COLLISION_FLAG::COL_RIGHT;
					else if (checkpoint2 == S_DOWN)
						flag2 = COLLISION_FLAG::COL_DOWN;
					else if (checkpoint2 == S_LEFT)
						flag2 = COLLISION_FLAG::COL_LEFT;
					checkpoint2 *= 25;


					if (Geometry::AabbAabb(RbA->GetAAbbMin()*0.5f + TrA->getTranslation() + checkpoint1, RbA->GetAAbbMax()*0.5f + TrA->getTranslation() + checkpoint1,
						RbB->GetAAbbMin() + TrB->getTranslation(), RbB->GetAAbbMax() + TrB->getTranslation()))
						CM1->SetCollisionFlag(flag1);

					if (Geometry::AabbAabb(RbB->GetAAbbMin()*0.5f + TrB->getTranslation() + checkpoint2, RbB->GetAAbbMax()*0.5f + TrB->getTranslation() + checkpoint2,
						RbA->GetAAbbMin() + TrA->getTranslation(), RbA->GetAAbbMax() + TrA->getTranslation()))
						CM2->SetCollisionFlag(flag2);

				}
				if ((a->GetEntityType() == EntityType::E_PLAYER1 || a->GetEntityType() == EntityType::E_PLAYER2) && b->GetEntityType() == EntityType::E_ITEM && !ItemUsed_b->GetItemUsed())
				{
					switch (ItemUsed_b->GetItemType())
					{
					case ItemType::E_ITEM_HP:
					{
						CharacterHP* characterHP = a->FindComponent<CharacterHP>();

						float HP = characterHP->GetHP() + 20.f;
						if (HP > 100.f)
							HP = 100.f;
						characterHP->SetHP(HP);
						Factory::CreateItemVFX(a, static_cast<ItemType>(ItemType::E_ITEM_HP));
						break;
					}
					case ItemType::E_ITEM_SPEED_UP:
					{
						CharacterMove* pCharMove = a->FindComponent<CharacterMove>();
						float speed = pCharMove->GetSpeed() + 20.f;
						if (speed > 120.f)
							speed = 120.f;
						pCharMove->SetSpeed(speed);
						Factory::CreateItemVFX(a, static_cast<ItemType>(ItemType::E_ITEM_HP));
						break;
					}
					case ItemType::E_ITEM_SPEED_DOWN:
					{
						CharacterMove* pCharMove = a->FindComponent<CharacterMove>();
						float speed = pCharMove->GetSpeed() - 20.f;
						if (speed < 40.f)
							speed = 40.f;
						pCharMove->SetSpeed(speed);
						Factory::CreateItemVFX(a, static_cast<ItemType>(ItemType::E_ITEM_HP));
						break;
					}
					default:
					{
						break;
					}
					}

					ItemUsed_b->SetItemUsed(true);

					
				}
				if (a->GetEntityType() == EntityType::E_ITEM && !ItemUsed_a->GetItemUsed() && (b->GetEntityType() == EntityType::E_PLAYER1 || b->GetEntityType() == EntityType::E_PLAYER2))
				{

					switch (ItemUsed_a->GetItemType())
					{
					case ItemType::E_ITEM_HP:
					{
						CharacterHP* characterHP = b->FindComponent<CharacterHP>();

						float HP = characterHP->GetHP() + 20.f;
						if (HP > 100.f)
							HP = 100.f;
						characterHP->SetHP(HP);
						Factory::CreateItemVFX(b, static_cast<ItemType>(ItemType::E_ITEM_HP));
						break;
					}
					case ItemType::E_ITEM_SPEED_UP:
					{
						CharacterMove* pCharMove = b->FindComponent<CharacterMove>();
						float speed = pCharMove->GetSpeed() + 20.f;
						if (speed > 120.f)
							speed = 120.f;
						pCharMove->SetSpeed(speed);
						Factory::CreateItemVFX(b, static_cast<ItemType>(ItemType::E_ITEM_HP));
						break;
					}
					case ItemType::E_ITEM_SPEED_DOWN:
					{
						CharacterMove* pCharMove = b->FindComponent<CharacterMove>();
						float speed = pCharMove->GetSpeed() - 20.f;
						if (speed < 40.f)
							speed = 40.f;
						pCharMove->SetSpeed(speed);
						Factory::CreateItemVFX(b, static_cast<ItemType>(ItemType::E_ITEM_HP));
						break;
					}
					default:
					{
						break;
					}
					}
					
					ItemUsed_a->SetItemUsed(true);


				}

					//// to check overlap between player1,2 and other object
					//if ((b->GetEntityType() == EntityType::E_SNOWBALL || b->GetEntityType() == EntityType::E_MAP1 )&&
					//	(a->GetEntityType() == EntityType::E_PLAYER1 || a->GetEntityType() == EntityType::E_PLAYER2))
					//{

					//	std::cout << "collision with " << a->GetName() << " with " << b->GetName() << std::endl;
					//	CharacterMove* CM = a->FindComponent<CharacterMove>();

					//	COLLISION_FLAG flag;

					//	glm::vec3 newPos = RbA->GetDirection();
					//	if (newPos == S_UP)
					//		flag = COLLISION_FLAG::COL_UP;
					//	else if (newPos == S_RIGHT)
					//		flag = COLLISION_FLAG::COL_RIGHT;
					//	else if (newPos == S_DOWN)
					//		flag = COLLISION_FLAG::COL_DOWN;
					//	else if (newPos == S_LEFT)
					//		flag = COLLISION_FLAG::COL_LEFT;
					//	newPos *= 10;

					//	if (Geometry::AabbAabb(RbB->GetAAbbMin() + TrB->getTranslation(), RbB->GetAAbbMax() + TrB->getTranslation(),
					//		RbA->GetAAbbMin() + TrA->getTranslation() + newPos, RbA->GetAAbbMax() + TrA->getTranslation() + newPos))
					//		CM->SetCollisionFlag(flag);
					//}

					// to check overlap between player1,2 and mapobject
					if ((a->GetEntityType() == EntityType::E_SNOWBALL || a->GetEntityType() == EntityType::E_MAP1) &&
						(b->GetEntityType() == EntityType::E_PLAYER1 || b->GetEntityType() == EntityType::E_PLAYER2))
					{
						std::cout << "collision with " << a->GetName() << " with " << b->GetName() << std::endl;

						CharacterMove* CM = b->FindComponent<CharacterMove>();

						COLLISION_FLAG flag;

						glm::vec3 newPos = RbB->GetDirection();
						if (newPos == S_UP)
							flag = COLLISION_FLAG::COL_UP;
						else if (newPos == S_RIGHT)
							flag = COLLISION_FLAG::COL_RIGHT;
						else if (newPos == S_DOWN)
							flag = COLLISION_FLAG::COL_DOWN;
						else if (newPos == S_LEFT)
							flag = COLLISION_FLAG::COL_LEFT;
						newPos *= 20;

						if (Geometry::AabbAabb(RbA->GetAAbbMin() + TrA->getTranslation(), RbA->GetAAbbMax() + TrA->getTranslation(),
							RbB->GetAAbbMin()*0.5f + TrB->getTranslation() + newPos, RbB->GetAAbbMax()*0.5f + TrB->getTranslation() + newPos))
							CM->SetCollisionFlag(flag);
					}

					// to check overlap between player1,2 and snowball
					if ((b->GetEntityType() == EntityType::E_SNOWBALL) &&
						(a->GetEntityType() == EntityType::E_PLAYER1 || a->GetEntityType() == EntityType::E_PLAYER2))
					{
						std::cout << "collision with " << a->GetName() << " with " << b->GetName() << std::endl;
						CharacterMove* CM = a->FindComponent<CharacterMove>();

						COLLISION_FLAG flag;

						glm::vec3 newPos = RbA->GetDirection();
						if (newPos == S_UP)
							flag = COLLISION_FLAG::COL_UP;
						else if (newPos == S_RIGHT)
							flag = COLLISION_FLAG::COL_RIGHT;
						else if (newPos == S_DOWN)
							flag = COLLISION_FLAG::COL_DOWN;
						else if (newPos == S_LEFT)
							flag = COLLISION_FLAG::COL_LEFT;
						newPos *= 10;

						if (Geometry::AabbAabb(RbB->GetAAbbMin() + TrB->getTranslation(), RbB->GetAAbbMax() + TrB->getTranslation(),
							RbA->GetAAbbMin()*0.5f + TrA->getTranslation() + newPos, RbA->GetAAbbMax()*0.5f + TrA->getTranslation() + newPos))
							CM->SetCollisionFlag(flag);
					}


					

					// press space to activate balls
					if (p_inputSystem->IsPressed(VK::KEY_RIGHT_SHIFT))
					{
						
						if (RbA->GetID() == COMPONENTTYPE::SNOW && RbB->GetID() == COMPONENTTYPE::CHARACTER2)
						{
							AudioSystem* p_audioSystem = app->getSystem<AudioSystem>();
							p_audioSystem->Play_Sound(SHOOT_SFX, false);
							RbA->SetIsActive(true);
							RbA->SetID(COMPONENTTYPE::SNOW2);
							RbA->SetVelocity(RbB->GetDirection() *= 100);
							RbA->SetDirection(RbB->GetDirection());
						}
						if (RbB->GetID() == COMPONENTTYPE::SNOW && RbA->GetID() == COMPONENTTYPE::CHARACTER2)
						{
							AudioSystem* p_audioSystem = app->getSystem<AudioSystem>();
							p_audioSystem->Play_Sound(SHOOT_SFX, false);
							RbB->SetIsActive(true);
							RbB->SetID(COMPONENTTYPE::SNOW2);
							RbB->SetVelocity(RbA->GetDirection() *= 100);
							RbB->SetDirection(RbA->GetDirection());
						}
					}
					if (p_inputSystem->IsPressed(VK::KEY_LEFT_SHIFT))
					{
						if (RbA->GetID() == COMPONENTTYPE::SNOW && RbB->GetID() == COMPONENTTYPE::CHARACTER1)
						{
							AudioSystem* p_audioSystem = app->getSystem<AudioSystem>();
							p_audioSystem->Play_Sound(SHOOT_SFX, false);
							RbA->SetIsActive(true);
							RbA->SetID(COMPONENTTYPE::SNOW1);
							RbA->SetVelocity(RbB->GetDirection() *= 100);
							RbA->SetDirection(RbB->GetDirection());
						}
						if (RbB->GetID() == COMPONENTTYPE::SNOW && RbA->GetID() == COMPONENTTYPE::CHARACTER1)
						{
							AudioSystem* p_audioSystem = app->getSystem<AudioSystem>();
							p_audioSystem->Play_Sound(SHOOT_SFX, false);
							RbB->SetIsActive(true);
							RbB->SetID(COMPONENTTYPE::SNOW1);
							RbB->SetVelocity(RbA->GetDirection() *= 100);
							RbB->SetDirection(RbA->GetDirection());
						}
					}
				//big snow
				// check which character build which ball
						if (RbA->GetID() == COMPONENTTYPE::CHARACTER1 && RbB->GetID() == COMPONENTTYPE::BIG_SNOW)
						{
							a->FindComponent<CharacterHP>()->SetHP(a->FindComponent<CharacterHP>()->GetHP() - 0.5f);
							RbA->SetKnockback(COL_BIG_SNOW);
							/*if (a->FindComponent<CharacterMove>()->GetCollidedObj() == nullptr)*/
								a->FindComponent<CharacterMove>()->SetCollidedObj(b);
							//					TrA->setTranslation(TrA->getTranslation()+RbB->GetVelocity());
							glm::vec3 newvel = glm::normalize(RbB->GetVelocity());
							newvel *= 10;
							Particle m_particle(TrA->getTranslation(), glm::vec3(newvel), glm::vec3(0));
							newvel *= 10;
							RbA->SetVelocity(newvel);

							m_particle.CreateMultipleParticle(a->FindComponent<TransformComponent>()->getTranslation(), Particle_Type::Snowball_Collision, 1, 12, -12);
						}
						if (RbA->GetID() == COMPONENTTYPE::CHARACTER2 && RbB->GetID() == COMPONENTTYPE::BIG_SNOW)
						{
							a->FindComponent<CharacterHP>()->SetHP(a->FindComponent<CharacterHP>()->GetHP() - 0.5f);
							RbA->SetKnockback(COL_BIG_SNOW);
							a->FindComponent<CharacterMove>()->SetCollidedObj(b);
							//			TrA->setTranslation(TrA->getTranslation() + RbB->GetVelocity());

							glm::vec3 newvel = glm::normalize(RbB->GetVelocity());
							newvel *= 10;
							Particle m_particle(TrA->getTranslation(), glm::vec3(newvel), glm::vec3(0));
							newvel *= 10;
							RbA->SetVelocity(newvel);
							m_particle.CreateMultipleParticle(a->FindComponent<TransformComponent>()->getTranslation(), Particle_Type::Snowball_Collision, 1, 12, -12);
						}
						if (RbB->GetID() == COMPONENTTYPE::CHARACTER2 && RbA->GetID() == COMPONENTTYPE::BIG_SNOW)
						{
							b->FindComponent<CharacterHP>()->SetHP(b->FindComponent<CharacterHP>()->GetHP() - 0.5f);

							RbB->SetKnockback(COL_BIG_SNOW);
							b->FindComponent<CharacterMove>()->SetCollidedObj(a);
							//				TrB->setTranslation(TrB->getTranslation() + RbA->GetVelocity());
							glm::vec3 newvel = glm::normalize(RbA->GetVelocity());
							newvel *= 10;
							Particle m_particle(TrB->getTranslation(), glm::vec3(newvel), glm::vec3(0));
							newvel *= 10;
							RbB->SetVelocity(newvel);
							m_particle.CreateMultipleParticle(a->FindComponent<TransformComponent>()->getTranslation(), Particle_Type::Snowball_Collision, 1, 12, -12);

						}
						if (RbB->GetID() == COMPONENTTYPE::CHARACTER1 && RbA->GetID() == COMPONENTTYPE::BIG_SNOW)
						{
							b->FindComponent<CharacterHP>()->SetHP(b->FindComponent<CharacterHP>()->GetHP() - 0.5f);
							RbB->SetKnockback(COL_BIG_SNOW);
							b->FindComponent<CharacterMove>()->SetCollidedObj(a);
							//			TrB->setTranslation(TrB->getTranslation() + RbA->GetVelocity());
							glm::vec3 newvel = glm::normalize(RbA->GetVelocity());
							newvel *= 10;
							Particle m_particle(TrB->getTranslation(), glm::vec3(newvel), glm::vec3(0));
							newvel *= 10;
							RbB->SetVelocity(newvel);
							m_particle.CreateMultipleParticle(a->FindComponent<TransformComponent>()->getTranslation(), Particle_Type::Snowball_Collision, 1, 12, -12);
						}


					// check which character build which ball
					if (RbA->GetKnockback() != COL_BIG_SNOW)
					{
						if (RbA->GetID() == COMPONENTTYPE::CHARACTER1 && RbB->GetID() == COMPONENTTYPE::SNOW2)
						{
							a->FindComponent<CharacterHP>()->SetHP(a->FindComponent<CharacterHP>()->GetHP() - 0.5f);
							RbA->SetKnockback(COL_SMALL_SNOW);
							a->FindComponent<CharacterMove>()->SetCollidedObj(b);
							//					TrA->setTranslation(TrA->getTranslation()+RbB->GetVelocity());
							glm::vec3 newvel = RbB->GetDirection();
							newvel *= 10;
							Particle m_particle(TrA->getTranslation(), glm::vec3(newvel), glm::vec3(0));
							newvel *= 10;
							RbA->SetVelocity(newvel);

							m_particle.CreateMultipleParticle(a->FindComponent<TransformComponent>()->getTranslation(), Particle_Type::Snowball_Collision, 1, 12, -12);
						}
						if (RbA->GetID() == COMPONENTTYPE::CHARACTER2 && RbB->GetID() == COMPONENTTYPE::SNOW1)
						{
							a->FindComponent<CharacterHP>()->SetHP(a->FindComponent<CharacterHP>()->GetHP() - 0.5f);
							RbA->SetKnockback(COL_SMALL_SNOW);
							a->FindComponent<CharacterMove>()->SetCollidedObj(b);
							//			TrA->setTranslation(TrA->getTranslation() + RbB->GetVelocity());

							glm::vec3 newvel = RbB->GetDirection();
							newvel *= 10;
							Particle m_particle(TrA->getTranslation(), glm::vec3(newvel), glm::vec3(0));
							newvel *= 10;
							RbA->SetVelocity(newvel);
							m_particle.CreateMultipleParticle(a->FindComponent<TransformComponent>()->getTranslation(), Particle_Type::Snowball_Collision, 1, 12, -12);
						}
					}
					if (RbB->GetKnockback() != COL_BIG_SNOW)
					{
						if (RbB->GetID() == COMPONENTTYPE::CHARACTER2 && RbA->GetID() == COMPONENTTYPE::SNOW1)
						{
							b->FindComponent<CharacterHP>()->SetHP(b->FindComponent<CharacterHP>()->GetHP() - 0.5f);

							RbB->SetKnockback(COL_SMALL_SNOW);
							b->FindComponent<CharacterMove>()->SetCollidedObj(a);
							//				TrB->setTranslation(TrB->getTranslation() + RbA->GetVelocity());
							glm::vec3 newvel = RbA->GetDirection();
							newvel *= 10;
							Particle m_particle(TrB->getTranslation(), glm::vec3(newvel), glm::vec3(0));
							newvel *= 10;
							RbB->SetVelocity(newvel);
							m_particle.CreateMultipleParticle(a->FindComponent<TransformComponent>()->getTranslation(), Particle_Type::Snowball_Collision, 1, 12, -12);

						}
						if (RbB->GetID() == COMPONENTTYPE::CHARACTER1 && RbA->GetID() == COMPONENTTYPE::SNOW2)
						{
							b->FindComponent<CharacterHP>()->SetHP(b->FindComponent<CharacterHP>()->GetHP() - 0.5f);
							RbB->SetKnockback(COL_SMALL_SNOW);
							b->FindComponent<CharacterMove>()->SetCollidedObj(a);
							//			TrB->setTranslation(TrB->getTranslation() + RbA->GetVelocity());
							glm::vec3 newvel = RbA->GetDirection();
							newvel *= 10;
							Particle m_particle(TrB->getTranslation(), glm::vec3(newvel), glm::vec3(0));
							newvel *= 10;
							RbB->SetVelocity(newvel);
							m_particle.CreateMultipleParticle(a->FindComponent<TransformComponent>()->getTranslation(), Particle_Type::Snowball_Collision, 1, 12, -12);
						}
					}
					if (RbB->GetID() == COMPONENTTYPE::CHARACTER1 && a->GetEntityType() == EntityType::E_MAP1)
					{
						RbB->SetKnockback(COL_NONE);
						b->FindComponent<CharacterMove>()->DeleteCollidedObj();
						RbA->SetVelocity(glm::vec3(0.f));
						RbB->SetVelocity(glm::vec3(0.f));
					}
					if (RbA->GetID() == COMPONENTTYPE::CHARACTER1 && b->GetEntityType() == EntityType::E_MAP1)
					{
						RbA->SetKnockback(COL_NONE);
						a->FindComponent<CharacterMove>()->DeleteCollidedObj();
						RbA->SetVelocity(glm::vec3(0.f));
						RbB->SetVelocity(glm::vec3(0.f));
					}
					if (RbB->GetID() == COMPONENTTYPE::CHARACTER2 && a->GetEntityType() == EntityType::E_MAP1)
					{
						RbB->SetKnockback(COL_NONE);
						b->FindComponent<CharacterMove>()->DeleteCollidedObj();
						RbA->SetVelocity(glm::vec3(0.f));
						RbB->SetVelocity(glm::vec3(0.f));
						
					}
					if (RbA->GetID() == COMPONENTTYPE::CHARACTER2 && b->GetEntityType() == EntityType::E_MAP1)
					{
						RbA->SetKnockback(COL_NONE);
						a->FindComponent<CharacterMove>()->DeleteCollidedObj();
						RbA->SetVelocity(glm::vec3(0.f));
						RbB->SetVelocity(glm::vec3(0.f));
					}
					if (RbA->GetID() == COMPONENTTYPE::SNOW1&& b->GetEntityType() == EntityType::E_MAP1)
					{
						//m_pES->DeleteEntity(a);
						a->SetAlive(false);
						Particle m_particle(TrA->getTranslation() - RbA->GetVelocity() * 0.5f, -RbA->GetVelocity() * 0.5f, glm::vec3(0.f));
						m_particle.CreateMultipleParticle(TrA->getTranslation(), Particle_Type::Collision_Ball_Map, 10, 25, 0);
						m_pAS->Play_Sound(VK::COLLISION_SFX, false);
						m_particle.CreateMultipleParticle(a->FindComponent<TransformComponent>()->getTranslation(), Particle_Type::Collision_Ball_Map, 10, 100, 50);
					}
					if (RbA->GetID() == COMPONENTTYPE::SNOW2&& b->GetEntityType() == EntityType::E_MAP1)
					{
						a->SetAlive(false);
						Particle m_particle(TrA->getTranslation(), -RbA->GetVelocity(), glm::vec3(0));
						m_particle.CreateMultipleParticle(TrA->getTranslation(), Particle_Type::Collision_Ball_Map, 10, 25, 0);
						m_pAS->Play_Sound(VK::COLLISION_SFX, false);

						m_particle.CreateMultipleParticle(a->FindComponent<TransformComponent>()->getTranslation(), Particle_Type::Collision_Ball_Map, 10, 100, 50);
						//m_pES->DeleteEntity(a);
					}
					if (RbB->GetID() == COMPONENTTYPE::SNOW1&& a->GetEntityType() == EntityType::E_MAP1)
					{
						b->SetAlive(false);
						m_pAS->Play_Sound(VK::COLLISION_SFX, false);
						//Particle m_particle(TrB->getTranslation(), -RbB->GetVelocity(), glm::vec3(0));
						Particle m_particle(TrB->getTranslation() - RbB->GetVelocity() * 0.5f, -RbB->GetVelocity() * 0.5f, glm::vec3(0.f));
						m_particle.CreateMultipleParticle(TrB->getTranslation(), Particle_Type::Collision_Ball_Map, 10, 25, 0);
						//m_particle.CreateMultipleParticle(b->FindComponent<TransformComponent>()->getTranslation(), 6, 10, 100, 50);
						//m_pES->DeleteEntity(b);
					}
					if (RbB->GetID() == COMPONENTTYPE::SNOW2&& a->GetEntityType() == EntityType::E_MAP1)
					{
						//todo working code
						b->SetAlive(false);
						m_pAS->Play_Sound(VK::COLLISION_SFX, false);

						Particle m_particle(TrB->getTranslation() - RbB->GetVelocity() * 0.5f, -RbB->GetVelocity() * 0.5f, glm::vec3(0.f));
						m_particle.CreateMultipleParticle(TrB->getTranslation() , Particle_Type::Collision_Ball_Map, 10, 25, 0);
						//m_pES->DeleteEntity(b);
					}
					return true;
				}
			}
			
			
		}

		return false;
	}

	void CollsionDatabase::RegisterCollsionTest(Shape::ShapeId a, Shape::ShapeId b, CollisionTest test)
	{
		CollsionRegistry[a][b] = test;
	}

	bool CollsionDatabase::GenerateContacts(Shape* shapeA, glm::vec3 poistionA, Shape* shapeB, glm::vec3 poistionB, ContactSet*c) const
	{
		return (*CollsionRegistry[shapeA->Id][shapeB->Id])(shapeA, poistionA, shapeB, poistionB, c);
	}
}
